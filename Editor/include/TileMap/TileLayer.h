#pragma once

#include <cstdint>

class TileLayer {
public:
	struct Tile {
		uint8_t id = -1;
	};

public:
	TileLayer() = default;
	~TileLayer();

	void Destroy();

	uint32_t GetTileCount() const { return width * height; }

public:
	Tile* tiles = nullptr;

	uint32_t width = 0;
	uint32_t height = 0;
};
