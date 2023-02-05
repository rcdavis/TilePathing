#include "TilePathing.h"

#include <queue>
#include <map>
#include <array>

#include "TileMap/TileMap.h"

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
    constexpr std::array<glm::uvec2, 4> neighbors = {
        glm::uvec2 { 0, -1 },
        glm::uvec2 { 0, 1 },
        glm::uvec2 { -1, 0 },
        glm::uvec2 { 1, 0 }
    };

    auto startNode = GetCell(startCoords);
    auto endNode = GetCell(endCoords);

    std::queue<Ref<Cell>> q;
    std::unordered_map<Ref<Cell>, Ref<Cell>> comeFrom;
    q.push(startNode);
    comeFrom.insert(std::make_pair(startNode, nullptr));

    while (!std::empty(q))
    {
        Ref<Cell> curNode = q.front();
        q.pop();

        if (curNode == endNode)
            break;

        for (int i = 0; i < std::size(neighbors); ++i)
        {
            const glm::uvec2 newCoords = curNode->coords + neighbors[i];
            if (newCoords.x >= 0 && newCoords.y >= 0 &&
                newCoords.x < mNumCols && newCoords.y < mNumRows)
            {
                Ref<Cell> newNode = GetCell(newCoords);
                if (comeFrom.find(newNode) == std::end(comeFrom))
                {
                    q.push(newNode);
                    comeFrom.insert(std::make_pair(newNode, curNode));
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

    mNumRows = tileMap->GetHeight();
    mNumCols = tileMap->GetWidth();
    mMap.resize((size_t)mNumRows * (size_t)mNumCols);

    for (uint32 row = 0; row < mNumRows; ++row)
    {
        for (uint32 col = 0; col < mNumCols; ++col)
        {
            mMap[((size_t)row * mNumCols) + col] = CreateRef<Cell>(glm::uvec2(row, col), 0);
        }
    }
}
