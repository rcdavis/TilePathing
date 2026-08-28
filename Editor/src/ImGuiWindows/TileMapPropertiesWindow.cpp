#include "ImGuiWindows/TileMapPropertiesWindow.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TileMapPropertiesWindow::TileMapPropertiesWindow(bool isOpen) :
	BaseImGuiWindow("Tile Map Properties", isOpen)
{}

void TileMapPropertiesWindow::OnRender() {
	ImGui::ColorEdit3("Start Color", glm::value_ptr(startColor));
	ImGui::ColorEdit3("End Color", glm::value_ptr(endColor));
	ImGui::ColorEdit3("Path Color", glm::value_ptr(pathColor));
	ImGui::ColorEdit3("Checked Color", glm::value_ptr(checkedColor));

	ImGui::Checkbox("Show Visited Tiles?", &showVisitedTiles);

	ImGui::Separator();

	ImGui::ColorEdit3("Selection Color", glm::value_ptr(selectionColor));
	ImGui::ColorEdit3("Movement Zone Color", glm::value_ptr(movementZoneColor));
}
