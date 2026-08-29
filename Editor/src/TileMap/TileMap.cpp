#include "TileMap/TileMap.h"

#include "Utils/Log.h"
#include "TileMap/TileSet.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileMapLoader.h"

#include "OpenGL/GLTexture.h"
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

	tileSets.reserve(tileMapData.tilesets.size());
	for (uint32_t i = 0; i < tileMapData.tilesets.size(); ++i) {
		TileSet tileSet;
		tileSet.firstGid = tileMapData.tilesets[i].firstGid;
		tileSet.texture = GLTexture::Load(Res::Textures::GetPath((Res::Textures::Id)tileMapData.tilesets[i].imageId));
		tileSet.tileWidth = tileMapData.tilesets[i].tileWidth;
		tileSet.tileHeight = tileMapData.tilesets[i].tileHeight;
		tileSet.tileCount = tileMapData.tilesets[i].tileCount;
		tileSet.columnCount = tileMapData.tilesets[i].columnCount;

		tileSet.terrains.resize(tileMapData.tilesets[i].movementCosts.size());
		for (uint32_t k = 0; k < tileMapData.tilesets[i].movementCosts.size(); ++k) {
			tileSet.terrains[k].movementCost = tileMapData.tilesets[i].movementCosts[k];
		}

		tileSets.emplace_back(tileSet);
	}

	tileLayers.reserve(tileMapData.layers.size());
	for (uint32_t i = 0; i < tileMapData.layers.size(); ++i) {
		TileLayer tileLayer;
		tileLayer.width = tileMapData.layers[i].width;
		tileLayer.height = tileMapData.layers[i].height;

		tileLayer.tiles.resize(tileMapData.layers[i].tiles.size());
		for (uint32_t k = 0; k < tileMapData.layers[i].tiles.size(); ++k) {
			tileLayer.tiles[k].id = tileMapData.layers[i].tiles[k];
		}

		tileLayers.emplace_back(tileLayer);
	}

	return true;
}

void TileMap::Destroy() {
	tileSets.clear();
	tileLayers.clear();
	width = 0;
	height = 0;
	tileWidth = 0;
	tileHeight = 0;
}
