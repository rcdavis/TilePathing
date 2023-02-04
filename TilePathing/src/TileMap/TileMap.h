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

    const std::vector<Ref<TileSet>>& GetTileSets() const { return mTileSets; }
    const std::vector<Ref<TileMapLayer>>& GetLayers() const { return mLayers; }
    const std::vector<Property>& GetProperties() const { return mProperties; }

    const std::string& GetName() const { return mName; }
    uint32 GetWidth() const { return mWidth; }
    uint32 GetHeight() const { return mHeight; }
    uint32 GetTileWidth() const { return mTileWidth; }
    uint32 GetTileHeight() const { return mTileHeight; }

    void SetName(const std::string& name) { mName = name; }
    void SetWidth(const uint32 width) { mWidth = width; }
    void SetHeight(const uint32 height) { mHeight = height; }
    void SetTileWidth(const uint32 width) { mTileWidth = width; }
    void SetTileHeight(const uint32 height) { mTileHeight = height; }

    static Ref<TileMap> Load(const std::filesystem::path& filepath);

private:
    std::vector<Ref<TileSet>> mTileSets;
    std::vector<Ref<TileMapLayer>> mLayers;
    std::vector<Property> mProperties;

    std::string mName;
    uint32 mWidth = 0;
    uint32 mHeight = 0;
    uint32 mTileWidth = 0;
    uint32 mTileHeight = 0;
};
