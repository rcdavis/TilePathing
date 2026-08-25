#include "TileMap/TileMap.h"

#include "Core.h"
#include "TileMap/TilePathing.h"
#include "Utils/Log.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileMapLoader.h"

#include "OpenGL/GLTexture.h"
#include "TextureIds.h"

#include <cstdint>
#include <pugixml.hpp>
#include <vector>

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
	tileMap->name = filepath.stem().string();
	tileMap->width = root.attribute("width").as_uint();
	tileMap->height = root.attribute("height").as_uint();
	tileMap->tileWidth = root.attribute("tilewidth").as_uint();
	tileMap->tileHeight = root.attribute("tileheight").as_uint();
	tileMap->properties = Property::LoadList(root.child("properties"));

	for (auto node = root.child("tileset"); node; node = node.next_sibling("tileset"))
		tileMap->tileSets.push_back(TileSet::Load(node));

	for (auto node = root.child("layer"); node; node = node.next_sibling("layer"))
		tileMap->layers.push_back(TileLayer::Load(node));

	return tileMap;
}

Ref<TileMap> TileMap::LoadBinary(const std::filesystem::path& filepath) {
	TileMapData tileMapData;
	if (!TileMapLoader::LoadBinary(filepath.c_str(), tileMapData)) {
		LOG_ERROR("Failed to parse tile map binary \"{0}\"", filepath.c_str());
		return nullptr;
	}

	auto tileMap = CreateRef<TileMap>();

	tileMap->name = filepath.stem().string();
	tileMap->width = tileMapData.width;
	tileMap->height = tileMapData.height;
	tileMap->tileWidth = tileMapData.tileWidth;
	tileMap->tileHeight = tileMapData.tileHeight;

	tileMap->tileSets.reserve(tileMapData.tilesets.size());
	for (uint32_t i = 0; i < tileMapData.tilesets.size(); ++i) {
		auto tileSet = CreateRef<TileSet>();
		tileSet->firstGid = tileMapData.tilesets[i].firstGid;
		tileSet->texture = GLTexture::Load(Res::Textures::GetPath((Res::Textures::Id)tileMapData.tilesets[i].imageId));
		tileSet->tileWidth = tileMapData.tilesets[i].tileWidth;
		tileSet->tileHeight = tileMapData.tilesets[i].tileHeight;
		tileSet->tileCount = tileMapData.tilesets[i].tileCount;
		tileSet->columnCount = tileMapData.tilesets[i].columnCount;

		tileSet->terrains.resize(tileMapData.tilesets[i].movementCosts.size());
		for (uint32_t k = 0; k < tileMapData.tilesets[i].movementCosts.size(); ++k) {
			tileSet->terrains[k].movementCost = tileMapData.tilesets[i].movementCosts[k];
		}

		tileMap->tileSets.emplace_back(tileSet);
	}

	tileMap->layers.reserve(tileMapData.layers.size());
	for (uint32_t i = 0; i < tileMapData.layers.size(); ++i) {
		auto tileLayer = CreateRef<TileLayer>();
		tileLayer->width = tileMapData.layers[i].width;
		tileLayer->height = tileMapData.layers[i].height;

		tileLayer->tiles.resize(tileMapData.layers[i].tiles.size());
		for (uint32_t k = 0; k < tileMapData.layers[i].tiles.size(); ++k) {
			tileLayer->tiles[k].mId = tileMapData.layers[i].tiles[k];
		}

		tileMap->layers.emplace_back(tileLayer);
	}

	return tileMap;
}
