
#include "CLI/CLI.hpp"

int main(int argc, char** argv) {
	CLI::App app("Program for building and converting game assets");
	app.set_version_flag("-V,--version", "1.0.0");

	CLI11_PARSE(app, argc, argv);

	return 0;
}
