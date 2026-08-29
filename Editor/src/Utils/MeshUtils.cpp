#include "Utils/MeshUtils.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileSet.h"

#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLVertexBuffer.h"
#include "OpenGL/GLIndexBuffer.h"

namespace MeshUtils {
	std::vector<Vertex> CreateTileMapVertices(Ref<TileMap> tileMap) {
		assert(tileMap && "Passing in a null tile map");
		assert(!std::empty(tileMap->tileSets) && "Tile map does not have a tile set");

		std::vector<Vertex> vertices;

		for (const TileLayer& tileLayer : tileMap->tileLayers) {
			const auto& tiles = tileLayer.tiles;
			for (uint32 i = 0; i < std::size(tiles); ++i) {
				const auto& tile = tiles[i];
				uint8_t tileSetIndex = std::numeric_limits<uint8_t>::max();
				for (uint8_t j = 0; j < tileMap->tileSets.size(); ++j) {
					if (tile.id >= tileMap->tileSets[j].firstGid) {
						tileSetIndex = j;
						break;
					}
				}
				assert(tileSetIndex != std::numeric_limits<uint8_t>::max() && "Tile does not have a tile set");

				const auto& tileSet = tileMap->tileSets[tileSetIndex];
				const uint32 tileWidth = tileSet.tileWidth;
				const uint32 tileHeight = tileSet.tileHeight;
				const uint32 numTilesWidth = tileLayer.width;
				const uint32 numTilesHeight = tileLayer.height;

				const uint32 xPos = ((i % numTilesWidth) * tileWidth);
				const uint32 yPos = (numTilesHeight * tileHeight) - ((i / numTilesWidth) * tileHeight);

				const std::array<glm::vec4, 4> vertPositions = {
					glm::vec4 { xPos, yPos, 0.0f, 1.0f },
					glm::vec4 { xPos + tileWidth, yPos, 0.0f, 1.0f },
					glm::vec4 { xPos + tileWidth, yPos - tileHeight, 0.0f, 1.0f },
					glm::vec4 { xPos, yPos - tileHeight, 0.0f, 1.0f }
				};

				const std::array<glm::vec2, 4> texCoords = tileSet.GetTexCoords(tile.id);

				for (int i = 0; i < 4; ++i)
					vertices.push_back({ vertPositions[i], texCoords[i] });
			}
		}

		return vertices;
	}

	Ref<GLVertexArray> CreateTileMapMesh(Ref<TileMap> tileMap) {
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
		for (size_t i = 0; i < std::size(quadIndices); i += 6) {
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

	Ref<GLVertexArray> CreateColoredTileMesh(Ref<TileMap> tileMap) {
		assert(tileMap && "Passing in a null tile map");

		auto vao = GLVertexArray::Create();
		vao->Bind();

		const uint32 numTilesHeight = tileMap->height;
		const uint32 tileWidth = tileMap->tileWidth;
		const uint32 tileHeight = tileMap->tileHeight;

		constexpr f32 xPos = 0.0f;
		const f32 yPos = (f32)(numTilesHeight * tileHeight);

		const auto vertices = CreateQuad(xPos, yPos, (f32)tileWidth, (f32)tileHeight);

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

	std::array<Vertex, 4> CreateQuad(f32 xPos, f32 yPos, f32 width, f32 height) {
		const std::array<glm::vec3, 4> vertPositions = {
			glm::vec3 { xPos, yPos, 0.0f },
			glm::vec3 { xPos + width, yPos, 0.0f },
			glm::vec3 { xPos + width, yPos - height, 0.0f },
			glm::vec3 { xPos, yPos - height, 0.0f }
		};

		constexpr std::array<glm::vec2, 4> texCoords = {
			glm::vec2 { 0.0f, 0.0f }, glm::vec2 { 1.0f, 0.0f },
			glm::vec2 { 1.0f, 1.0f }, glm::vec2 { 0.0f, 1.0f }
		};

		return std::array<Vertex, 4> {
			Vertex { vertPositions[0], texCoords[0] },
			Vertex { vertPositions[1], texCoords[1] },
			Vertex { vertPositions[2], texCoords[2] },
			Vertex { vertPositions[3], texCoords[3] }
		};
	}
}
