#include "TileMap/TileMap.h"

#include "Utils/Log.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileMapLoader.h"

#include "TextureIds.h"

TileMap::~TileMap() {
	Destroy();
}

bool TileMap::Load(const char* const filepath) {
	TileMapData tileMapData;
	if (!TileMapLoader::LoadBinary(filepath, tileMapData)) {
		LOG_ERROR("Failed to parse tile map binary \"{0}\"", filepath);
		return false;
	}

	Destroy();

	width = tileMapData.width;
	height = tileMapData.height;
	tileWidth = tileMapData.tileWidth;
	tileHeight = tileMapData.tileHeight;

	tileSetCount = (uint16_t)tileMapData.tilesets.size();
	tileSets = new TileSet[tileSetCount];
	for (uint16_t i = 0; i < tileSetCount; ++i) {
		tileSets[i].firstGid = tileMapData.tilesets[i].firstGid;
		tileSets[i].textureId = (Res::Textures::Id)tileMapData.tilesets[i].imageId;
		tileSets[i].tileWidth = tileMapData.tilesets[i].tileWidth;
		tileSets[i].tileHeight = tileMapData.tilesets[i].tileHeight;
		tileSets[i].tileCount = tileMapData.tilesets[i].tileCount;
		tileSets[i].columnCount = tileMapData.tilesets[i].columnCount;

		tileSets[i].terrainCount = (uint8_t)tileMapData.tilesets[i].movementCosts.size();
		tileSets[i].terrains = new TileSet::Terrain[tileSets[i].terrainCount];
		for (uint16_t k = 0; k < tileSets[i].terrainCount; ++k) {
			tileSets[i].terrains[k].movementCost = tileMapData.tilesets[i].movementCosts[k];
		}
	}

	tileLayerCount = (uint16_t)tileMapData.layers.size();
	tileLayers = new TileLayer[tileLayerCount];
	for (uint16_t i = 0; i < tileLayerCount; ++i) {
		tileLayers[i].width = tileMapData.layers[i].width;
		tileLayers[i].height = tileMapData.layers[i].height;

		uint32_t tileCount = (uint32_t)tileMapData.layers[i].tiles.size();
		tileLayers[i].tiles = new TileLayer::Tile[tileCount];
		for (uint32_t k = 0; k < tileCount; ++k) {
			tileLayers[i].tiles[k].id = tileMapData.layers[i].tiles[k];
		}
	}

	return true;
}

void TileMap::Destroy() {
	delete[] tileSets;
	tileSets = nullptr;
	tileSetCount = 0;

	delete[] tileLayers;
	tileLayers = nullptr;
	tileLayerCount = 0;

	width = 0;
	height = 0;
	tileWidth = 0;
	tileHeight = 0;
}
