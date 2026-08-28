#pragma once

#include <glm/glm.hpp>

#include "ImGuiWindows/BaseImGuiWindow.h"

class TileMapPropertiesWindow : public BaseImGuiWindow {
public:
	TileMapPropertiesWindow(bool isOpen = false);

	void OnRender() override;

public:
	glm::vec4 startColor{ 0.0f, 0.0f, 1.0f, 0.5f };
	glm::vec4 endColor{ 1.0f, 0.0f, 0.0f, 0.5f };
	glm::vec4 pathColor{ 0.0f, 1.0f, 1.0f, 0.5f };
	glm::vec4 checkedColor{ 0.5f, 0.5f, 0.5f, 0.5f };

	glm::vec4 selectionColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 movementZoneColor{ 0.0f, 0.0f, 1.0f, 0.5f };

	bool showVisitedTiles = false;
};
