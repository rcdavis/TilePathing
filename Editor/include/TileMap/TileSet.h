#pragma once

#include "Core.h"

#include <string>
#include <vector>
#include <array>

#include <glm/glm.hpp>

class GLTexture;

class TileSet {
public:
	struct Terrain {
		uint8_t mTileId = -1;
		uint8_t movementCost = 1;
	};

public:
	TileSet() = default;

	std::array<glm::vec2, 4> GetTexCoords(const uint32_t tileId);
	const Terrain& GetTerrain(const uint32_t tileId);

public:
	Ref<GLTexture> texture;
	std::vector<Terrain> terrains;

	uint32_t firstGid = 1;
	uint32_t tileWidth = 0;
	uint32_t tileHeight = 0;
	uint32_t tileCount = 0;
	uint32_t columnCount = 0;
};
