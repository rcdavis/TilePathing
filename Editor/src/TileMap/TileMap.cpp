#include "TileMap/TileMap.h"

#include "Core.h"
#include "Utils/Log.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"

#include <cstdint>
#include <pugixml.hpp>
#include <fstream>
#include <vector>

struct TileMapBinHeader {
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t tileWidth = 0;
	uint32_t tileHeight = 0;
	uint32_t tilesetCount = 0;
	uint32_t layerCount = 0;
};

struct TileSetData {
	uint32_t tileWidth = 0;
	uint32_t tileHeight = 0;
	uint32_t tileCount = 0;
	uint32_t columnCount = 0;
	std::vector<uint8_t> movementCosts;
};

struct TileLayerData {
	uint32_t width = 0;
	uint32_t height = 0;
	std::vector<uint8_t> tiles;
};

struct TileMapData {
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t tileWidth = 0;
	uint32_t tileHeight = 0;
	std::vector<TileSetData> tilesets;
	std::vector<TileLayerData> layers;
};

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
	std::ifstream file(filepath, std::ios::binary);
	if (!file) {
		LOG_ERROR("Failed to load tile map binary \"{}\"", filepath.c_str());
		return nullptr;
	}

	TileMapBinHeader header;
	file.read((char*)&header, sizeof(TileMapBinHeader));

	TileMapData tileMapData;
	tileMapData.width = header.width;
	tileMapData.height = header.height;
	tileMapData.tileWidth = header.tileWidth;
	tileMapData.tileHeight = header.tileHeight;

	for (uint32_t i = 0; i < header.tilesetCount; ++i) {
		TileSetData tilesetData;
		file.read((char*)&tilesetData.tileWidth, sizeof(uint32_t));
		file.read((char*)&tilesetData.tileHeight, sizeof(uint32_t));
		file.read((char*)&tilesetData.tileCount, sizeof(uint32_t));
		file.read((char*)&tilesetData.columnCount, sizeof(uint32_t));

		tilesetData.movementCosts.resize(tilesetData.tileCount);
		file.read((char*)tilesetData.movementCosts.data(), tilesetData.movementCosts.size());

		tileMapData.tilesets.emplace_back(tilesetData);
	}

	for (uint32_t i = 0; i < header.layerCount; ++i) {
		TileLayerData layerData;
		file.read((char*)&layerData.width, sizeof(uint32_t));
		file.read((char*)&layerData.height, sizeof(uint32_t));

		layerData.tiles.resize(layerData.width * layerData.height);
		file.read((char*)layerData.tiles.data(), layerData.tiles.size());

		tileMapData.layers.emplace_back(layerData);
	}

	return nullptr;
}
