#pragma once

#include "TextureIds.h"

#include <array>

#include <glm/glm.hpp>

class TileSet {
public:
	struct Terrain {
		uint8_t movementCost = 1;
	};

public:
	TileSet() = default;
	~TileSet();

	std::array<glm::vec2, 4> GetTexCoords(const uint8_t tileId) const;
	const Terrain& GetTerrain(const uint8_t tileId) const;

	void Destroy();

public:
	Terrain* terrains = nullptr;
	uint8_t terrainCount = 0;

	Res::Textures::Id textureId = Res::Textures::Id::Count;

	uint8_t firstGid = 1;
	uint8_t tileWidth = 0;
	uint8_t tileHeight = 0;
	uint8_t tileCount = 0;
	uint8_t columnCount = 0;
};
