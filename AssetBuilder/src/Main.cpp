
#include "CLI/CLI.hpp"

#include "pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <regex>
#include <fstream>

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

static void ConvertTilemap(const std::string& tilemapPath, const std::string& tilesetPath, const std::string& outputPath);

int main(int argc, char** argv) {
	CLI::App app("Program for building and converting game assets");
	app.set_version_flag("-V,--version", "1.0.0");

	std::string tilemapFile;
	std::string tilesetFile;
	std::string outputFile;

	auto* tilemaps = app.add_subcommand("tilemaps", "Convert tilemap assets");
	tilemaps->add_option("-m,--tilemap", tilemapFile, "Input Tiled tile map (tmx)")->required();
	tilemaps->add_option("-s,--tileset", tilesetFile, "Input Tiled tile set (tsx)")->required();
	tilemaps->add_option("-o,--output", outputFile, "Output tile map binary")->required();

	CLI11_PARSE(app, argc, argv);

	if (tilemaps->parsed()) {
		ConvertTilemap(tilemapFile, tilesetFile, outputFile);
	}

	return 0;
}

static void ConvertTilemap(const std::string& tilemapPath, const std::string& tilesetPath, const std::string& outputPath) {
	pugi::xml_document doc;
	if (const auto result = doc.load_file(tilemapPath.c_str()); !result) {
		std::cerr << "Failed to load tile map \"" << tilemapPath << "\": " << result.description() << std::endl;
		return;
	}

	TileMapData tileMapData;
	const pugi::xml_node mapNode = doc.child("map");
	tileMapData.width = mapNode.attribute("width").as_uint();
	tileMapData.height = mapNode.attribute("height").as_uint();
	tileMapData.tileWidth = mapNode.attribute("tilewidth").as_uint();
	tileMapData.tileHeight = mapNode.attribute("tileheight").as_uint();

	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode; layerNode = layerNode.next_sibling("layer")) {
		TileLayerData layerData;
		layerData.width = layerNode.attribute("width").as_uint();
		layerData.height = layerNode.attribute("height").as_uint();

		const pugi::xml_node dataNode = layerNode.child("data");
    	if (strcmp(dataNode.attribute("encoding").as_string(), "csv") == 0) {
			const std::string csvText = dataNode.text().as_string();
			std::vector<std::string> splitValues;
			const std::regex reg("[, \n]");
			std::copy(
				std::sregex_token_iterator(std::cbegin(csvText), std::cend(csvText), reg, -1),
				std::sregex_token_iterator(),
				std::back_inserter(splitValues)
			);

			for (const auto& tileId : splitValues) {
				layerData.tiles.emplace_back((uint8_t)atoi(tileId.c_str()));
			}
		}

		tileMapData.layers.emplace_back(layerData);
	}

	if (const auto result = doc.load_file(tilesetPath.c_str()); !result) {
		std::cerr << "Failed to load tile set \"" << tilesetPath << "\": " << result.description() << std::endl;
		return;
	}

	TileSetData tilesetData;
    const pugi::xml_node tilesetNode = doc.child("tileset");
	tilesetData.name = tilesetNode.attribute("name").as_string();
	tilesetData.tileWidth = tilesetNode.attribute("tilewidth").as_uint();
	tilesetData.tileHeight = tilesetNode.attribute("tileheight").as_uint();
	tilesetData.tileCount = tilesetNode.attribute("tilecount").as_uint();
	tilesetData.columnCount = tilesetNode.attribute("columns").as_uint();
	tilesetData.movementCosts.resize(tilesetData.tileCount, 1);

	for (pugi::xml_node tileNode = tilesetNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile")) {
        const uint8_t tileId = (uint8_t)tileNode.attribute("id").as_uint();
		const pugi::xml_node propertiesNode = tileNode.child("properties");
		for (pugi::xml_node propNode = propertiesNode.child("property"); propNode; propNode = propNode.next_sibling("property")) {
			const auto name = propNode.attribute("name").as_string();
			const auto val = propNode.attribute("value");
			if (strcmp(name, "movementCost") == 0) {
				tilesetData.movementCosts[tileId] = (uint8_t)val.as_uint(1);
			}
		}
	}

	tileMapData.tilesets.emplace_back(tilesetData);

	std::ofstream file(outputPath, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to create file \"" << outputPath << "\"" << std::endl;
		return;
	}

	file << tileMapData.width << tileMapData.height;
	file << tileMapData.tileWidth << tileMapData.tileHeight;

	file << (uint32_t)tileMapData.tilesets.size();
	file << (uint32_t)tileMapData.layers.size();

	for (const TileSetData& tileSet : tileMapData.tilesets) {
		file << tileSet.tileWidth << tileSet.tileHeight;
		file << tileSet.tileCount;
		file << tileSet.columnCount;

		file.write((const char*)tileSet.movementCosts.data(), tileSet.movementCosts.size());
	}

	for (const TileLayerData& layerData : tileMapData.layers) {
		file << layerData.width << layerData.height;

		file.write((const char*)layerData.tiles.data(), layerData.tiles.size());
	}

	std::cout << "Converted tile map \"" << tilemapPath << "\" to \"" << outputPath << "\"" << std::endl;
}
