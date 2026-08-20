
#include "CLI/CLI.hpp"

#include "pugixml.hpp"

struct TileSetData {
	std::string name;
	uint32_t tileWidth = 0;
	uint32_t tileHeight = 0;
	uint32_t tileCount = 0;
	uint32_t columnCount = 0;
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
	std::cout << "Converting tilemap:\n";
	std::cout << "  Tilemap: " << tilemapPath << std::endl;
	std::cout << "  Tileset: " << tilesetPath << std::endl;
	std::cout << "  Output: " << outputPath << std::endl;

	std::cout << "Parsing tileset..." << std::endl;

	pugi::xml_document doc;
	if (const auto result = doc.load_file(tilesetPath.c_str()); !result) {
		std::cerr << "Failed to load tile set \"" << tilesetPath << "\": " << result.description() << std::endl;
		return;
	}

	TileSetData tilesetData;
    const auto tilesetNode = doc.child("tileset");
	tilesetData.name = tilesetNode.attribute("name").as_string();
	tilesetData.tileWidth = tilesetNode.attribute("tilewidth").as_uint();
	tilesetData.tileHeight = tilesetNode.attribute("tileheight").as_uint();
	tilesetData.tileCount = tilesetNode.attribute("tilecount").as_uint();
	tilesetData.columnCount = tilesetNode.attribute("columns").as_uint();

	std::cout << "Tileset data:\n";
	std::cout << "  Name: " << tilesetData.name << std::endl;
	std::cout << "  Tile Width: " << tilesetData.tileWidth << std::endl;
	std::cout << "  Tile Height: " << tilesetData.tileHeight << std::endl;
	std::cout << "  Tile Count: " << tilesetData.tileCount << std::endl;
	std::cout << "  Column Count: " << tilesetData.columnCount << std::endl;
}
