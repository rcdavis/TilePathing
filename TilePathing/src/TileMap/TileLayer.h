#pragma once

#include "Core.h"
#include "TileMapLayer.h"

#include <vector>

namespace pugi
{
    class xml_node;
}

class TileLayer : public TileMapLayer
{
public:
    struct Tile
    {
        uint32 mId = -1;
    };

public:
    TileLayer() : TileMapLayer(TileMapLayer::Type::Tile) {}

    const std::vector<Tile>& GetTiles() const { return mTiles; }

    uint32 GetWidth() const { return mWidth; }
    uint32 GetHeight() const { return mHeight; }

    static Ref<TileLayer> Load(const pugi::xml_node& node);

private:
    std::vector<Tile> mTiles;
    uint32 mWidth = 0;
    uint32 mHeight = 0;
};
