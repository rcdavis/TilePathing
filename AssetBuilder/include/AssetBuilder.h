#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

class AssetBuilder {
public:
	struct TmbinHeader {
		char magic[4] = { 'T', 'M', 'B', '1' };
		uint32_t version = 1;
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t tileWidth = 0;
		uint32_t tileHeight = 0;
		uint32_t tilesetCount = 0;
		uint32_t layerCount = 0;
	};

	struct TileSetData {
		std::string name;
		uint32_t imageId = -1;
		uint32_t tileWidth = 0;
		uint32_t tileHeight = 0;
		uint32_t tileCount = 0;
		uint32_t columnCount = 0;
		std::vector<uint8_t> movementCosts;
	};

	struct TileLayerData {
		uint32_t width = 0;
		uint32_t height = 0;
		std::vector<uint8_t> tiles;
	};

	struct TileMapData {
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t tileWidth = 0;
		uint32_t tileHeight = 0;
		std::vector<TileSetData> tilesets;
		std::vector<TileLayerData> layers;
	};

public:
	AssetBuilder() = default;

	void BuildAssets(const std::filesystem::path& inputDir, const std::filesystem::path& outputDir, const std::filesystem::path& generatedDir);

	void BuildTextures(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir);
	void BuildShaders(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir);
	void BuildTiles(const std::filesystem::path& inputDir, const std::filesystem::path& outputDir, const std::filesystem::path& generatedDir);

private:
	void ParseTiledMap(const std::filesystem::path& tilemapPath, const std::filesystem::path& inputDir, TileMapData& tileMapData);
	void CreateTileMapBinary(const std::filesystem::path& tilemapPath, const TileMapData& tileMapData);

	void CreateTextureIdHeader(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir);
	void CreateShaderIdHeader(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir);
	void CreateTileIdHeader(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir);

private:
	std::vector<std::filesystem::path> mTextures;
	std::vector<std::filesystem::path> mShaders;
	std::vector<std::filesystem::path> mTilemaps;
	std::vector<std::filesystem::path> mTilesets;
};
