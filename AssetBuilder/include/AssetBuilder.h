#pragma once

#include <vector>
#include <filesystem>

class AssetBuilder {
public:
	struct TileSetData {
		std::string name;
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

	void BuildAssets(const std::filesystem::path& inputDir, const std::filesystem::path& outputDir);

	void BuildTextures(const std::filesystem::path& inputDir);

	void ConvertTilemap(const std::filesystem::path& tilemapPath, const std::filesystem::path& tilesetPath, const std::filesystem::path& outputPath);

private:
	std::vector<std::filesystem::path> mTextures;
};
