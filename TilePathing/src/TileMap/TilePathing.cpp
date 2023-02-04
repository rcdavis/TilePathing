#include "TilePathing.h"

#include <queue>
#include <unordered_set>
#include <set>

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
    //for (int i = 0; i < std::size(mMap); ++i)
      //  mMap[i]->parent = nullptr;

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

        if (coords.y > 0 && !visited.count({ coords.x, coords.y - 1 }))
        {
            glm::uvec2 newCoords{ coords.x, coords.y - 1 };
            q.push(newCoords);
            visited.insert(newCoords);
            auto newCell = GetCell(newCoords);
            newCell->parent = GetCell(coords);
        }

        if (coords.y < mNumRows && !visited.count({ coords.x, coords.y + 1 }))
        {
            glm::uvec2 newCoords{ coords.x, coords.y + 1 };
            q.push(newCoords);
            visited.insert(newCoords);
            auto newCell = GetCell(newCoords);
            newCell->parent = GetCell(coords);
        }

        if (coords.x > 0 && !visited.count({ coords.x - 1, coords.y }))
        {
            glm::uvec2 newCoords{ coords.x - 1, coords.y };
            q.push(newCoords);
            visited.insert(newCoords);
            auto newCell = GetCell(newCoords);
            newCell->parent = GetCell(coords);
        }

        if (coords.x < mNumCols && !visited.count({ coords.x + 1, coords.y }))
        {
            glm::uvec2 newCoords{ coords.x + 1, coords.y };
            q.push(newCoords);
            visited.insert(newCoords);
            auto newCell = GetCell(newCoords);
            newCell->parent = GetCell(coords);
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

    for (int row = 0; row < mNumRows; ++row)
    {
        for (int col = 0; col < mNumCols; ++col)
        {
            mMap[((size_t)row * mNumCols) + col] = CreateRef<Cell>(glm::uvec2(row, col), 0);
        }
    }
}
