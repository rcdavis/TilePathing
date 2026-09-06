#pragma once

#include <vector>

#include <glm/glm.hpp>

struct TileMapPathsWindow {
	static constexpr const char* Title = "Tile Map Paths";

	struct Path {
		glm::uvec2 start{ 0, 0 };
		glm::uvec2 end{ 0, 0 };
	};

	std::vector<Path> paths;
	uint32_t curSelected = 0;
	bool isOpen = true;

	void Render();

	void RenderMenuItem();
};
