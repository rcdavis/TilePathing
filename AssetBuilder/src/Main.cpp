
#include "CLI/CLI.hpp"

#include "AssetBuilder.h"

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

	AssetBuilder builder;

	if (tilemaps->parsed()) {
		builder.ConvertTilemap(tilemapFile, tilesetFile, outputFile);
	}

	return 0;
}
