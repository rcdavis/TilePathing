#include "MeshUtils.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileSet.h"

#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLVertexBuffer.h"
#include "OpenGL/GLIndexBuffer.h"

namespace MeshUtils
{
    std::vector<Vertex> CreateTileMapVertices(Ref<TileMap> tileMap)
    {
        assert(tileMap && "Passing in a null tile map");
        assert(!std::empty(tileMap->GetTileSets()) && "Tile map does not have a tile set");

        std::vector<Vertex> vertices;

        for (const Ref<TileMapLayer>& layer : tileMap->GetLayers())
        {
            if (layer->GetType() != TileMapLayer::Type::Tile)
                continue;

            const auto& tileLayer = DynamicCastRef<TileLayer>(layer);
            const auto& tiles = tileLayer->GetTiles();
            for (uint32 i = 0; i < std::size(tiles); ++i)
            {
                const auto& tile = tiles[i];
                Ref<TileSet> tileSet;
                for (const auto& ts : tileMap->GetTileSets())
                {
                    if (tile.mId >= ts->GetFirstGid())
                    {
                        tileSet = ts;
                        break;
                    }
                }

                assert(tileSet && "Tile Layer does not have a tile set");

                const uint32 tileWidth = tileSet->GetTileWidth();
                const uint32 tileHeight = tileSet->GetTileHeight();
                const uint32 numTilesWidth = tileLayer->GetWidth();
                const uint32 numTilesHeight = tileLayer->GetHeight();

                const uint32 xPos = ((i % numTilesWidth) * tileWidth);
                const uint32 yPos = (numTilesHeight * tileHeight) - ((i / numTilesWidth) * tileHeight);

                const std::array<glm::vec4, 4> vertPositions = {
                    glm::vec4 { xPos, yPos, 0.0f, 1.0f },
                    glm::vec4 { xPos + tileWidth, yPos, 0.0f, 1.0f },
                    glm::vec4 { xPos + tileWidth, yPos - tileHeight, 0.0f, 1.0f },
                    glm::vec4 { xPos, yPos - tileHeight, 0.0f, 1.0f }
                };

                const std::array<glm::vec2, 4> texCoords = tileSet->GetTexCoords(tile.mId);

                for (int i = 0; i < 4; ++i)
                    vertices.push_back({ vertPositions[i], texCoords[i] });
            }
        }

        return vertices;
    }

    Ref<GLVertexArray> CreateTileMapMesh(Ref<TileMap> tileMap)
    {
        const auto vertices = CreateTileMapVertices(tileMap);

        auto vao = GLVertexArray::Create();
        vao->Bind();

        auto vb = GLVertexBuffer::Create((uint32)std::size(vertices) * sizeof(Vertex));
        vb->SetData(std::data(vertices), (uint32)std::size(vertices) * sizeof(Vertex));
        vb->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        vao->AddVertexBuffer(vb);

        std::vector<uint16> quadIndices(std::size(vertices) / 4 * 6);

        uint16 offset = 0;
        for (size_t i = 0; i < std::size(quadIndices); i += 6)
        {
            quadIndices[i] = offset;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset;

            offset += 4;
        }

        auto quadIB = GLIndexBuffer::Create(std::data(quadIndices), (uint32)std::size(quadIndices));
        vao->SetIndexBuffer(quadIB);

        vao->Unbind();
        return vao;
    }

    Ref<GLVertexArray> CreateColoredTileMesh(Ref<TileMap> tileMap)
    {
        assert(tileMap && "Passing in a null tile map");

        auto vao = GLVertexArray::Create();
        vao->Bind();

        const uint32 numTilesHeight = tileMap->GetHeight();
        const uint32 tileWidth = tileMap->GetTileWidth();
        const uint32 tileHeight = tileMap->GetTileHeight();

        constexpr f32 xPos = 0.0f;
        const uint32 yPos = (numTilesHeight * tileHeight);

        const std::array<glm::vec4, 4> vertPositions = {
            glm::vec4 { xPos, yPos, 0.0f, 1.0f },
            glm::vec4 { xPos + tileWidth, yPos, 0.0f, 1.0f },
            glm::vec4 { xPos + tileWidth, yPos - tileHeight, 0.0f, 1.0f },
            glm::vec4 { xPos, yPos - tileHeight, 0.0f, 1.0f }
        };

        const std::array<Vertex, 4> vertices = {
            Vertex { vertPositions[0], glm::vec2(0.0f) },
            Vertex { vertPositions[1], glm::vec2(0.0f) },
            Vertex { vertPositions[2], glm::vec2(0.0f) },
            Vertex { vertPositions[3], glm::vec2(0.0f) }
        };

        auto vb = GLVertexBuffer::Create((uint32)std::size(vertices) * sizeof(Vertex));
        vb->SetData(std::data(vertices), (uint32)std::size(vertices) * sizeof(Vertex));
        vb->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        });
        vao->AddVertexBuffer(vb);

        constexpr std::array<uint16, 6> indices = {
            0, 1, 2, 2, 3, 0
        };

        auto quadIB = GLIndexBuffer::Create(std::data(indices), (uint32)std::size(indices));
        vao->SetIndexBuffer(quadIB);

        vao->Unbind();
        return vao;
    }
}
