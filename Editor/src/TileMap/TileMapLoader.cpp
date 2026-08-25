#include "TileMap/TileMapLoader.h"

#include "Utils/Log.h"

#include <fstream>

namespace TileMapLoader {
	struct TmbinHeader {
		char magic[4] = { 'T', 'M', 'B', '1' };
		uint32_t version = 1;
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t tileWidth = 0;
		uint32_t tileHeight = 0;
		uint32_t tilesetCount = 0;
		uint32_t layerCount = 0;
	};

	bool LoadBinary(const char* const filepath, TileMapData& tileMapData) {
		std::ifstream file(filepath, std::ios::binary);
		if (!file) {
			LOG_ERROR("Failed to load tile map binary \"{}\"", filepath);
			return false;
		}

		TmbinHeader header;
		file.read((char*)&header, sizeof(TmbinHeader));

		tileMapData.width = header.width;
		tileMapData.height = header.height;
		tileMapData.tileWidth = header.tileWidth;
		tileMapData.tileHeight = header.tileHeight;

		for (uint32_t i = 0; i < header.tilesetCount; ++i) {
			TileSetData tilesetData;
			file.read((char*)&tilesetData.imageId, sizeof(uint32_t));
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

		return true;
	}
}
