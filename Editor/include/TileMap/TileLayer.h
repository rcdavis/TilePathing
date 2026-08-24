#pragma once

#include "Core.h"
#include "TileMapLayer.h"

#include <cstdint>
#include <vector>

namespace pugi {
    class xml_node;
}

class TileLayer : public TileMapLayer
{
public:
    struct Tile {
        uint32_t mId = -1;
    };

public:
    TileLayer() : TileMapLayer(TileMapLayer::Type::Tile) {}

    static Ref<TileLayer> Load(const pugi::xml_node& node);

public:
    std::vector<Tile> tiles;
    uint32_t width = 0;
    uint32_t height = 0;
};
