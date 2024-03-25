#include "TilePathing.h"

#include <queue>
#include <unordered_map>
#include <array>

#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"

TilePathing::TilePathing(Ref<TileMap> tileMap) :
    mMap(),
    mVisitedCoords(),
    mNumRows(0),
    mNumCols(0)
{
    CreateMap(tileMap);
}

std::vector<glm::uvec2> TilePathing::FindPath(glm::uvec2 startCoords, glm::uvec2 endCoords)
{
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

    typedef std::pair<uint32, Ref<Cell>> QElement;
    std::priority_queue<QElement, std::vector<QElement>, std::greater<QElement>> q;
    std::unordered_map<Ref<Cell>, Ref<Cell>> comeFrom;
    std::unordered_map<Ref<Cell>, uint32> costSoFar;
    mVisitedCoords.clear();
    mVisitedCoords.insert(startCoords);

    q.emplace(0, startNode);
    comeFrom[startNode] = nullptr;
    costSoFar[startNode] = 0;

    while (!std::empty(q))
    {
        Ref<Cell> curNode = q.top().second;
        q.pop();

        if (curNode == endNode)
            break;

        for (int i = 0; i < std::size(neighbors); ++i)
        {
            const glm::uvec2 newCoords = curNode->coords + neighbors[i];
            if (IsInBounds(newCoords))
            {
                Ref<Cell> newNode = GetCell(newCoords);
                const uint32 cost = costSoFar[curNode] + newNode->cost;
                if (costSoFar.find(newNode) == std::end(costSoFar) || cost < costSoFar[newNode])
                {
                    q.emplace(cost + Heuristic(newCoords, endCoords), newNode);
                    costSoFar[newNode] = cost;
                    comeFrom[newNode] = curNode;
                    mVisitedCoords.insert(newNode->coords);
                }
            }
        }
    }

    std::vector<glm::uvec2> path;
    while (endNode)
    {
        path.push_back(endNode->coords);
        endNode = comeFrom[endNode];
    }

    std::reverse(std::begin(path), std::end(path));

    return path;
}

TilePathing::Zone TilePathing::FindMovementZone(glm::uvec2 coords, uint32 movementSteps)
{
    if (!IsInBounds(coords))
        return {};

    constexpr std::array<glm::uvec2, 4> neighbors = {
        glm::uvec2 { 0, -1 },
        glm::uvec2 { 0, 1 },
        glm::uvec2 { -1, 0 },
        glm::uvec2 { 1, 0 }
    };

    auto startNode = GetCell(coords);

    typedef std::pair<uint32, Ref<Cell>> QElement;
    std::priority_queue<QElement, std::vector<QElement>, std::greater<QElement>> q;
    std::unordered_map<Ref<Cell>, Ref<Cell>> comeFrom;
    std::unordered_map<Ref<Cell>, uint32> costSoFar;
    mVisitedCoords.clear();
    mVisitedCoords.insert(coords);

    q.emplace(0, startNode);
    comeFrom[startNode] = nullptr;
    costSoFar[startNode] = 0;

    while (!std::empty(q))
    {
        Ref<Cell> curNode = q.top().second;
        q.pop();

        for (int i = 0; i < std::size(neighbors); ++i)
        {
            const glm::uvec2 newCoords = curNode->coords + neighbors[i];
            if (IsInBounds(newCoords))
            {
                Ref<Cell> newNode = GetCell(newCoords);
                const uint32 cost = costSoFar[curNode] + newNode->cost;
                if (cost < movementSteps + 1 && (costSoFar.find(newNode) == std::end(costSoFar) || cost < costSoFar[newNode]))
                {
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
        zone.mTiles.push_back(coords);

    return zone;
}

uint32 TilePathing::Heuristic(glm::uvec2 start, glm::uvec2 end) const
{
    return std::abs((long)end.x - (long)start.x) + std::abs((long)end.y - (long)start.y);
}

void TilePathing::CreateMap(Ref<TileMap> tileMap)
{
    assert(tileMap && "Tile map is null");
    assert(!std::empty(tileMap->GetTileSets()) && "Tile map does not have a tile set");

    Ref<TileLayer> tileLayer;
    for (const auto& layer : tileMap->GetLayers())
    {
        if (layer->GetType() == TileMapLayer::Type::Tile)
        {
            tileLayer = DynamicCastRef<TileLayer>(layer);
            break;
        }
    }
    assert(tileLayer && "Tile map does not have a tile layer");

    mNumRows = tileMap->GetHeight();
    mNumCols = tileMap->GetWidth();
    mMap.resize((size_t)mNumRows * (size_t)mNumCols);

    for (uint32 row = 0; row < mNumRows; ++row)
    {
        for (uint32 col = 0; col < mNumCols; ++col)
        {
            const auto& tile = tileLayer->GetTiles()[((uint64)row * tileLayer->GetWidth()) + col];
            Ref<TileSet> tileSet;
            for (const auto& ts : tileMap->GetTileSets())
            {
                if (tile.mId >= ts->GetFirstGid())
                {
                    tileSet = ts;
                    break;
                }
            }
            assert(tileSet && "Tile does not have a tile set");
            const auto& props = tileSet->GetTerrain(tile.mId).mProperties;
            auto iter = std::find_if(std::cbegin(props), std::cend(props), [](const Property& p)
                {
                    return strcmp(std::data(p.GetName()), "movementCost") == 0;
                });
            const uint32 movementCost = (iter != std::cend(props)) ? (uint32)iter->AsInt32() : 1;

            mMap[((size_t)row * mNumCols) + col] = CreateRef<Cell>(glm::uvec2(col, row), movementCost);
        }
    }
}
