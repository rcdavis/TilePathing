#include "TileMap/TilePathing.h"

#include <limits>
#include <queue>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <string.h>

#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"

TilePathing::TilePathing(TileMap& tileMap) :
	mMap(),
	mVisitedCoords(),
	mNumRows(0),
	mNumCols(0)
{
	CreateMap(tileMap);
}

std::vector<glm::uvec2> TilePathing::FindPath(glm::uvec2 startCoords, glm::uvec2 endCoords) {
	if (!IsInBounds(startCoords) || !IsInBounds(endCoords))
		return {};

	constexpr std::array<glm::uvec2, 4> neighbors = {
		glm::uvec2 { 0, -1 },
		glm::uvec2 { 0, 1 },
		glm::uvec2 { -1, 0 },
		glm::uvec2 { 1, 0 }
	};

	auto startNode = GetCell(startCoords);
	auto endNode = GetCell(endCoords);

	typedef std::pair<uint32_t, Ref<Cell>> QElement;
	std::priority_queue<QElement, std::vector<QElement>, std::greater<QElement>> q;
	std::unordered_map<Ref<Cell>, Ref<Cell>> comeFrom;
	std::unordered_map<Ref<Cell>, uint32_t> costSoFar;
	mVisitedCoords.clear();
	mVisitedCoords.insert(startCoords);

	q.emplace(0, startNode);
	comeFrom[startNode] = nullptr;
	costSoFar[startNode] = 0;

	while (!std::empty(q)) {
		Ref<Cell> curNode = q.top().second;
		q.pop();

		if (curNode == endNode)
			break;

		for (int i = 0; i < std::size(neighbors); ++i) {
			const glm::uvec2 newCoords = curNode->coords + neighbors[i];
			if (IsInBounds(newCoords)) {
				Ref<Cell> newNode = GetCell(newCoords);
				const uint32_t cost = costSoFar[curNode] + newNode->cost;
				if (costSoFar.find(newNode) == std::end(costSoFar) || cost < costSoFar[newNode]) {
					q.emplace(cost + Heuristic(newCoords, endCoords), newNode);
					costSoFar[newNode] = cost;
					comeFrom[newNode] = curNode;
					mVisitedCoords.insert(newNode->coords);
				}
			}
		}
	}

	std::vector<glm::uvec2> path;
	while (endNode) {
		path.push_back(endNode->coords);
		endNode = comeFrom[endNode];
	}

	std::reverse(std::begin(path), std::end(path));

	return path;
}

TilePathing::Zone TilePathing::FindMovementZone(glm::uvec2 coords, uint32_t movementSteps) {
	if (!IsInBounds(coords))
		return {};

	constexpr std::array<glm::uvec2, 4> neighbors = {
		glm::uvec2 { 0, -1 },
		glm::uvec2 { 0, 1 },
		glm::uvec2 { -1, 0 },
		glm::uvec2 { 1, 0 }
	};

	auto startNode = GetCell(coords);

	typedef std::pair<uint32_t, Ref<Cell>> QElement;
	std::priority_queue<QElement, std::vector<QElement>, std::greater<QElement>> q;
	std::unordered_map<Ref<Cell>, Ref<Cell>> comeFrom;
	std::unordered_map<Ref<Cell>, uint32_t> costSoFar;
	mVisitedCoords.clear();
	mVisitedCoords.insert(coords);

	q.emplace(0, startNode);
	comeFrom[startNode] = nullptr;
	costSoFar[startNode] = 0;

	while (!std::empty(q)) {
		Ref<Cell> curNode = q.top().second;
		q.pop();

		for (int i = 0; i < std::size(neighbors); ++i) {
			const glm::uvec2 newCoords = curNode->coords + neighbors[i];
			if (IsInBounds(newCoords)) {
				Ref<Cell> newNode = GetCell(newCoords);
				const uint32_t cost = costSoFar[curNode] + newNode->cost;
				if (cost < movementSteps + 1 && (costSoFar.find(newNode) == std::end(costSoFar) || cost < costSoFar[newNode])) {
					q.emplace(cost + Heuristic(coords, newCoords), newNode);
					costSoFar[newNode] = cost;
					comeFrom[newNode] = curNode;
					mVisitedCoords.insert(newNode->coords);
				}
			}
		}
	}

	Zone zone;
	for (auto& coords : mVisitedCoords)
		zone.tiles.push_back(coords);

	return zone;
}

uint32_t TilePathing::Heuristic(glm::uvec2 start, glm::uvec2 end) const {
	return std::abs((long)end.x - (long)start.x) + std::abs((long)end.y - (long)start.y);
}

void TilePathing::CreateMap(TileMap& tileMap) {
	assert(!std::empty(tileMap.tileSets) && "Tile map does not have a tile set");

	uint8_t tileLayerIndex = std::numeric_limits<uint8_t>::max();
	for (uint8_t i = 0; i < tileMap.tileLayers.size(); ++i) {
		if (!std::empty(tileMap.tileLayers[i].tiles)) {
			tileLayerIndex = i;
			break;
		}
	}

	assert(tileLayerIndex != std::numeric_limits<uint8_t>::max() && "Tile map does not have a valid tile layer index");

	const auto& tileLayer = tileMap.tileLayers[tileLayerIndex];

	mNumRows = tileMap.height;
	mNumCols = tileMap.width;
	mMap.resize((size_t)mNumRows * (size_t)mNumCols);

	for (uint32_t row = 0; row < mNumRows; ++row) {
		for (uint32_t col = 0; col < mNumCols; ++col) {
			const auto& tile = tileLayer.tiles[(row * tileLayer.width) + col];
			uint8_t tileSetIndex = std::numeric_limits<uint8_t>::max();
			for (uint8_t i = 0; i < tileMap.tileSets.size(); ++i) {
				if (tile.id >= tileMap.tileSets[i].firstGid) {
					tileSetIndex = i;
					break;
				}
			}
			assert(tileSetIndex != std::numeric_limits<uint8_t>::max() && "Tile does not have a tile set");

			mMap[((size_t)row * mNumCols) + col] = CreateRef<Cell>(
				glm::uvec2(col, row),
				tileMap.tileSets[tileSetIndex].GetTerrain(tile.id).movementCost);
		}
	}
}
