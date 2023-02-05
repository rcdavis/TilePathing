#pragma once

#include "Core.h"

#include <vector>
#include <glm/glm.hpp>

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
    uint32 mNumRows = 0;
    uint32 mNumCols = 0;
};
