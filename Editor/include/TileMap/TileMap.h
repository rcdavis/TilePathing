#pragma once

#include "Core.h"

#include <filesystem>
#include <vector>

class TileSet;
class TileLayer;

class TileMap {
public:
	TileMap() = default;

	static Ref<TileMap> LoadBinary(const std::filesystem::path& filepath);

public:
	std::vector<Ref<TileSet>> tileSets;
	std::vector<Ref<TileLayer>> tileLayers;

	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t tileWidth = 0;
	uint16_t tileHeight = 0;
};
