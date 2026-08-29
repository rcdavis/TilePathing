#pragma once

#include "Core.h"

#include <filesystem>
#include <vector>

class TileSet;
class TileLayer;

class TileMap {
public:
	TileMap() = default;
	~TileMap();

	static Ref<TileMap> LoadBinary(const std::filesystem::path& filepath);

	bool Load(const char* const filepath);

	void Destroy();

public:
	std::vector<TileSet> tileSets;
	std::vector<TileLayer> tileLayers;

	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t tileWidth = 0;
	uint16_t tileHeight = 0;
};
