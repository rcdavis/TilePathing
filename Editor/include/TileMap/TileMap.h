#pragma once

#include "Core.h"
#include "Property.h"

#include <filesystem>
#include <vector>

class TileSet;
class TileMapLayer;

class TileMap {
public:
    TileMap() = default;

    static Ref<TileMap> LoadBinary(const std::filesystem::path& filepath);

public:
    std::vector<Ref<TileSet>> tileSets;
    std::vector<Ref<TileMapLayer>> layers;
    std::vector<Property> properties;

    std::string name;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t tileWidth = 0;
    uint32_t tileHeight = 0;
};
