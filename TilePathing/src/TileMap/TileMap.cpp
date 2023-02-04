#include "TileMap.h"

#include "Log.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"

#include <pugixml.hpp>

Ref<TileMap> TileMap::Load(const std::filesystem::path& filepath)
{
    pugi::xml_document doc;
    const auto result = doc.load_file(filepath.string().c_str());
    if (!result)
    {
        LOG_ERROR("Failed to load tile map \"{0}\": {1}", filepath.string(), result.description());
        return nullptr;
    }

    const auto root = doc.child("map");

    auto tileMap = CreateRef<TileMap>();
    tileMap->mName = filepath.stem().string();
    tileMap->mWidth = root.attribute("width").as_uint();
    tileMap->mHeight = root.attribute("height").as_uint();
    tileMap->mProperties = Property::LoadList(root.child("properties"));

    for (auto node = root.child("tileset"); node; node = node.next_sibling("tileset"))
        tileMap->mTileSets.push_back(TileSet::Load(node));

    for (auto node = root.child("layer"); node; node = node.next_sibling("layer"))
        tileMap->mLayers.push_back(TileLayer::Load(node));

    return tileMap;
}
