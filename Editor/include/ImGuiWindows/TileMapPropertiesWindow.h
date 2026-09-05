#pragma once

#include <glm/glm.hpp>

struct TileMapPropertiesWindow {
	static constexpr const char* Title = "Tile Map Properties";

	glm::vec4 startColor{ 0.0f, 0.0f, 1.0f, 0.5f };
	glm::vec4 endColor{ 1.0f, 0.0f, 0.0f, 0.5f };
	glm::vec4 pathColor{ 0.0f, 1.0f, 1.0f, 0.5f };
	glm::vec4 checkedColor{ 0.5f, 0.5f, 0.5f, 0.5f };

	glm::vec4 selectionColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 movementZoneColor{ 0.0f, 0.0f, 1.0f, 0.5f };

	bool showVisitedTiles = false;
	bool isOpen = true;

	void Render();

	void RenderMenuItem();
};
