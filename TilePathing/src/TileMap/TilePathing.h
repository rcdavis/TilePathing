#pragma once

#include "Core.h"

#include <unordered_set>
#include <vector>
#include <glm/glm.hpp>

template<>
struct std::hash<glm::uvec2>
{
    std::size_t operator()(glm::uvec2 v) const noexcept
    {
        const size_t h1 = std::hash<size_t>{}(v.x);
        const size_t h2 = std::hash<size_t>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

class TileMap;

class TilePathing
{
public:
    struct Cell
    {
        glm::uvec2 coords{ 0, 0 };
        uint32 cost = 0;

        Cell() = default;
        Cell(glm::uvec2 coords, uint32 cost) :
            coords(coords), cost(cost) {}
    };

public:
    TilePathing() = default;
    TilePathing(Ref<TileMap> tileMap);

    void SetTileMap(Ref<TileMap> tileMap) { CreateMap(tileMap); }
    std::unordered_set<glm::uvec2> GetVisitedCoords() { return mVisitedCoords; }

    std::vector<glm::uvec2> FindPath(glm::uvec2 startCoords, glm::uvec2 endCoords);

private:
    Ref<Cell> GetCell(glm::uvec2 coords) { return mMap[((size_t)coords.y * mNumCols) + coords.x]; }
    bool IsInBounds(glm::uvec2 coords) const {
        return coords.x >= 0 && coords.y >= 0 && coords.x < mNumCols&& coords.y < mNumRows;
    }

    void CreateMap(Ref<TileMap> tileMap);
    uint32 Heuristic(glm::uvec2 start, glm::uvec2 end) const;

private:
    std::vector<Ref<Cell>> mMap;
    std::unordered_set<glm::uvec2> mVisitedCoords;
    uint32 mNumRows = 0;
    uint32 mNumCols = 0;
};
