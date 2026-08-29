#pragma once

#include "TileMapLayer.h"

#include <vector>

class TileLayer : public TileMapLayer {
public:
	struct Tile {
		uint8_t id = -1;
	};

public:
	TileLayer() : TileMapLayer(TileMapLayer::Type::Tile) {}

public:
	std::vector<Tile> tiles;
	uint32_t width = 0;
	uint32_t height = 0;
};
