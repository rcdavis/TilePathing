#pragma once

#include <cstdint>

class TileSet;
class TileLayer;

class TileMap {
public:
	TileMap() = default;
	~TileMap();

	bool Load(const char* const filepath);

	void Destroy();

public:
	TileSet* tileSets = nullptr;
	TileLayer* tileLayers = nullptr;

	uint16_t tileSetCount = 0;
	uint16_t tileLayerCount = 0;

	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t tileWidth = 0;
	uint16_t tileHeight = 0;
};
