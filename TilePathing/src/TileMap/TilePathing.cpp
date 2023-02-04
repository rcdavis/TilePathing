#include "TilePathing.h"

#include <queue>
#include <unordered_set>
#include <set>
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

    std::queue<glm::uvec2> q;
    std::unordered_set<glm::uvec2> visited;
    q.push(startCoords);
    visited.insert(startCoords);

    while (!std::empty(q))
    {
        const glm::uvec2 coords = q.front();
        q.pop();

        if (coords == endCoords)
            break;

        for (int i = 0; i < std::size(neighbors); ++i)
        {
            const glm::uvec2 newCoords = coords + neighbors[i];
            if (newCoords.x >= 0 && newCoords.y >= 0 &&
                newCoords.x < mNumCols && newCoords.y < mNumRows && !visited.count(newCoords))
            {
                q.push(newCoords);
                visited.insert(newCoords);
                auto newCell = GetCell(newCoords);
                newCell->parent = GetCell(coords);
            }
        }
    }

    auto curCell = GetCell(endCoords);
    std::vector<Cell> path;
    while (curCell)
    {
        path.push_back(Cell(curCell->coords, curCell->cost));
        curCell = curCell->parent;
    }

    std::reverse(std::begin(path), std::end(path));

    return path;
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
