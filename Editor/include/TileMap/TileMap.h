#pragma once

#include "Core.h"
#include "Property.h"

#include <filesystem>
#include <vector>

class TileSet;
class TileMapLayer;

class TileMap
{
public:
    TileMap() = default;

    static Ref<TileMap> Load(const std::filesystem::path& filepath);
    static Ref<TileMap> LoadBinary(const std::filesystem::path& filepath);

public:
    std::vector<Ref<TileSet>> tileSets;
    std::vector<Ref<TileMapLayer>> layers;
    std::vector<Property> properties;

    std::string name;
    uint32 width = 0;
    uint32 height = 0;
    uint32 tileWidth = 0;
    uint32 tileHeight = 0;
};
