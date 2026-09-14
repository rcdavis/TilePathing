#include "TileMap/TileSet.h"

#include "TextureSystem.h"

TileSet::~TileSet() {
	Destroy();
}

std::array<glm::vec2, 4> TileSet::GetTexCoords(const uint8_t tileId) const {
	const uint32_t texWidth = TextureSystem::GetWidth(textureId);
	const uint32_t texHeight = TextureSystem::GetHeight(textureId);
	const float texCoordU = (((tileId - firstGid) % columnCount) * tileWidth) / (float)texWidth;
	const float texCoordV = (((tileId - firstGid) / columnCount) * tileHeight) / (float)texHeight;

	const std::array<glm::vec2, 4> texCoords = {
		glm::vec2 { texCoordU, 1.0f - texCoordV },
		glm::vec2 { texCoordU + (tileWidth / (float)texWidth), 1.0f - texCoordV },
		glm::vec2 { texCoordU + (tileWidth / (float)texWidth), 1.0f - (texCoordV + (tileHeight / (float)texHeight)) },
		glm::vec2 { texCoordU, 1.0f - (texCoordV + (tileHeight / (float)texHeight)) }
	};

	return texCoords;
}

const TileSet::Terrain& TileSet::GetTerrain(const uint8_t tileId) const {
	assert(tileId - firstGid >= 0 && tileId - firstGid < terrainCount && "Invalid tileId");
	return terrains[tileId - firstGid];
}

void TileSet::Destroy() {
	delete[] terrains;
	terrains = nullptr;
	terrainCount = 0;

	textureId = Res::Textures::Id::Count;

	firstGid = 1;
	tileWidth = 0;
	tileHeight = 0;
	tileCount = 0;
	columnCount = 0;
}
