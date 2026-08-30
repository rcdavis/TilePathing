#pragma once

#include "Core.h"

#include <array>
#include <vector>

#include <glm/glm.hpp>

class TileMap;
class GLVertexArray;

namespace MeshUtils {
	struct Vertex {
		glm::vec3 position{ 0.0f };
		glm::vec2 texCoord{ 0.0f };
	};

	std::vector<Vertex> CreateTileMapVertices(TileMap& tileMap);
	Ref<GLVertexArray> CreateTileMapMesh(TileMap& tileMap);

	Ref<GLVertexArray> CreateColoredTileMesh(TileMap& tileMap);

	std::array<Vertex, 4> CreateQuad(float xPos, float yPos, float width, float height);
}
