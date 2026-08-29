#pragma once

#include "Core.h"

#include <vector>
#include <array>

#include <glm/glm.hpp>

class GLTexture;

class TileSet {
public:
	struct Terrain {
		uint8_t movementCost = 1;
	};

public:
	TileSet() = default;

	std::array<glm::vec2, 4> GetTexCoords(const uint8_t tileId);
	const Terrain& GetTerrain(const uint8_t tileId);

public:
	Ref<GLTexture> texture;
	std::vector<Terrain> terrains;

	uint8_t firstGid = 1;
	uint8_t tileWidth = 0;
	uint8_t tileHeight = 0;
	uint8_t tileCount = 0;
	uint8_t columnCount = 0;
};
