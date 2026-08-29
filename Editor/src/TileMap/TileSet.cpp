#include "TileMap/TileSet.h"

#include "OpenGL/GLTexture.h"

std::array<glm::vec2, 4> TileSet::GetTexCoords(const uint8_t tileId) {
	const uint32_t texWidth = texture->GetWidth();
	const uint32_t texHeight = texture->GetHeight();
	const f32 texCoordU = (((tileId - firstGid) % columnCount) * tileWidth) / (f32)texWidth;
	const f32 texCoordV = (((tileId - firstGid) / columnCount) * tileHeight) / (f32)texHeight;

	const std::array<glm::vec2, 4> texCoords = {
		glm::vec2 { texCoordU, 1.0f - texCoordV },
		glm::vec2 { texCoordU + (tileWidth / (f32)texWidth), 1.0f - texCoordV },
		glm::vec2 { texCoordU + (tileWidth / (f32)texWidth), 1.0f - (texCoordV + (tileHeight / (f32)texHeight)) },
		glm::vec2 { texCoordU, 1.0f - (texCoordV + (tileHeight / (f32)texHeight)) }
	};

	return texCoords;
}

const TileSet::Terrain& TileSet::GetTerrain(const uint8_t tileId) {
	assert(tileId - firstGid >= 0 && tileId - firstGid < std::size(terrains) && "Invalid tileId");
	return terrains[tileId - firstGid];
}
