#pragma once

#include <cstdint>
#include <vector>

class TileLayer {
public:
	struct Tile {
		uint8_t id = -1;
	};

public:
	TileLayer() = default;

public:
	std::vector<Tile> tiles;
	uint32_t width = 0;
	uint32_t height = 0;
};
