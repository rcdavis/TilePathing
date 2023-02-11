#pragma once

#include "Core.h"

#include <vector>

#include <glm/glm.hpp>

class TileMap;
class GLVertexArray;

namespace MeshUtils
{
    struct Vertex
    {
        glm::vec3 position{ 0.0f };
        glm::vec2 texCoord{ 0.0f };
    };

    std::vector<Vertex> CreateTileMapVertices(Ref<TileMap> tileMap);
    Ref<GLVertexArray> CreateTileMapMesh(Ref<TileMap> tileMap);

    Ref<GLVertexArray> CreateColoredTileMesh(Ref<TileMap> tileMap);
}
