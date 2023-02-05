#include "TilePathing.h"

#include <queue>
#include <map>
#include <array>

#include "TileMap/TileMap.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"

template<>
struct std::hash<glm::uvec2>
{
    std::size_t operator()(glm::uvec2 v) const noexcept
    {
        size_t h1 = std::hash<size_t>{}(v.x);
        size_t h2 = std::hash<size_t>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

TilePathing::TilePathing(Ref<TileMap> tileMap) :
    mMap(),
    mNumRows(0),
    mNumCols(0)
{
    CreateMap(tileMap);
}

std::vector<TilePathing::Cell> TilePathing::FindPath(glm::uvec2 startCoords, glm::uvec2 endCoords)
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

    //std::priority_queue<Ref<Cell>, std::vector<Ref<Cell>>, std::greater<Ref<Cell>>> q;
    typedef std::pair<uint32, Ref<Cell>> QElement;
    std::priority_queue<QElement, std::vector<QElement>, std::greater<QElement>> q;
    std::unordered_map<Ref<Cell>, Ref<Cell>> comeFrom;
    std::unordered_map<Ref<Cell>, uint32> costSoFar;

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
                    q.emplace(cost, newNode);
                    costSoFar[newNode] = cost;
                    comeFrom[newNode] = curNode;
                }
            }
        }
    }

    std::vector<Cell> path;
    while (endNode)
    {
        path.push_back(Cell(endNode->coords, endNode->cost));
        endNode = comeFrom[endNode];
    }

    std::reverse(std::begin(path), std::end(path));

    return path;
}

void TilePathing::SetHeuristicFunc(HeuristicFunc func)
{
    mHeuristicFunc = std::bind(func, std::placeholders::_1, std::placeholders::_2);
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
                    return p.GetName() == "movementCost";
                });
            const uint32 movementCost = (iter != std::cend(props)) ? (uint32)iter->AsInt32() : 1;

            mMap[((size_t)row * mNumCols) + col] = CreateRef<Cell>(glm::uvec2(col, row), movementCost);
        }
    }
}
