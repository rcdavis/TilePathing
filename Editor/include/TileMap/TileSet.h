#pragma once

#include "Core.h"
#include "Property.h"

#include <cstdint>
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
    struct Terrain {
        uint32_t mTileId = -1;
		uint8_t movementCost = 1;
        std::vector<Property> mProperties;
    };

public:
    TileSet() = default;

    std::array<glm::vec2, 4> GetTexCoords(const uint32 tileId);
    const Terrain& GetTerrain(const uint32 tileId);

    static Ref<TileSet> Load(const pugi::xml_node& node);

public:
    std::string name;
    Ref<GLTexture> texture;
    std::vector<Terrain> terrains;
    std::vector<Property> properties;

    uint32 firstGid = 1;
    uint32 tileWidth = 0;
    uint32 tileHeight = 0;
    uint32 tileCount = 0;
    uint32 columnCount = 0;
};
