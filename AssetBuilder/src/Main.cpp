
#include "CLI/CLI.hpp"

int main(int argc, char** argv) {
	CLI::App app("Asset Builder");
	app.add_flag("--verbose", "Enable verbose output");
	CLI11_PARSE(app, argc, argv);

	return 0;
}
