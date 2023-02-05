#pragma once

#include "Core.h"

#include <functional>
#include <vector>
#include <numeric>
#include <glm/glm.hpp>

class TileMap;

class TilePathing
{
public:
    using HeuristicFunc = std::function<uint32(glm::vec2, glm::vec2)>;

    struct Cell
    {
        Ref<Cell> parent = nullptr;
        glm::uvec2 coords{ std::numeric_limits<uint32>::max() };
        uint32 cost = 0;

        Cell() = default;
        Cell(glm::uvec2 coords, uint32 cost, Ref<Cell> parent = nullptr) :
            parent(parent), coords(coords), cost(cost) {}

        friend bool operator<(const Cell& lhs, const Cell& rhs)
        {
            return lhs.cost < rhs.cost;
        }
    };

public:
    TilePathing() = default;
    TilePathing(Ref<TileMap> tileMap);

    void SetTileMap(Ref<TileMap> tileMap) { CreateMap(tileMap); }
    void SetHeuristicFunc(HeuristicFunc func);

    Ref<Cell> GetCell(glm::uvec2 coords) { return mMap[((size_t)coords.y * mNumCols) + coords.x]; }

    std::vector<Cell> FindPath(glm::uvec2 startCoords, glm::uvec2 endCoords);

private:
    void CreateMap(Ref<TileMap> tileMap);

private:
    std::vector<Ref<Cell>> mMap;
    HeuristicFunc mHeuristicFunc;
    uint32 mNumRows = 0;
    uint32 mNumCols = 0;
};
