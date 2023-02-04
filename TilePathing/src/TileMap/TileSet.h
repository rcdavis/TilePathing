#pragma once

#include "Core.h"
#include "Property.h"

#include <string>
#include <vector>
#include <array>

#include <glm/glm.hpp>

namespace pugi
{
    class xml_node;
}

class GLTexture;

class TileSet
{
public:
    struct Terrain
    {
        std::string mName;
        uint32 mTileId = -1;
        std::vector<Property> mProperties;
    };

public:
    TileSet() = default;

    const std::string& GetName() const { return mName; }
    const Ref<GLTexture>& GetTexture() const { return mTexture; }
    const std::vector<Terrain>& GetTerrains() const { return mTerrains; }
    const std::vector<Property>& GetProperties() const { return mProperties; }

    uint32 GetFirstGid() const { return mFirstGid; }
    uint32 GetTileWidth() const { return mTileWidth; }
    uint32 GetTileHeight() const { return mTileHeight; }
    uint32 GetTileCount() const { return mTileCount; }
    uint32 GetColumnCount() const { return mColumnCount; }

    std::array<glm::vec2, 4> GetTexCoords(const uint32 tileId);

    static Ref<TileSet> Load(const pugi::xml_node& node);

private:
    std::string mName;
    Ref<GLTexture> mTexture;
    std::vector<Terrain> mTerrains;
    std::vector<Property> mProperties;

    uint32 mFirstGid = 1;
    uint32 mTileWidth = 0;
    uint32 mTileHeight = 0;
    uint32 mTileCount = 0;
    uint32 mColumnCount = 0;
};
