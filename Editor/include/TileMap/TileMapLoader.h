#pragma once

#include <cstdint>
#include <vector>

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

namespace TileMapLoader {
	bool LoadBinary(const char* const filepath, TileMapData& tileMapData);
}
