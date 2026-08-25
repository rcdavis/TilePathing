
#include "CLI/CLI.hpp"

#include "AssetBuilder.h"

int main(int argc, char** argv) {
	std::string inputDir;
	std::string outputDir;

	CLI::App app("Program for building and converting game assets");
	app.set_version_flag("-V,--version", "1.0.0");
	app.add_option("-i,--input", inputDir, "Input directory containing assets")->required();
	app.add_option("-o,--output", outputDir, "Output directory for built assets")->required();

	CLI11_PARSE(app, argc, argv);

	AssetBuilder builder;
	builder.BuildAssets(inputDir, outputDir);

	return 0;
}
