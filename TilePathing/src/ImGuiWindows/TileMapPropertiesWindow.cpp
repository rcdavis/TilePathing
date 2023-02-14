#include "TileMapPropertiesWindow.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TileMapPropertiesWindow::TileMapPropertiesWindow(bool isOpen) :
    ImGuiWindow("Tile Map Properties", isOpen)
{}

void TileMapPropertiesWindow::OnRender()
{
    ImGui::ColorEdit3("Start Color", glm::value_ptr(mStartColor));
    ImGui::ColorEdit3("End Color", glm::value_ptr(mEndColor));
    ImGui::ColorEdit3("Path Color", glm::value_ptr(mPathColor));
    ImGui::ColorEdit3("Checked Color", glm::value_ptr(mCheckedColor));
    ImGui::Checkbox("Show Visited Tiles?", &mShowVisitedTiles);
}
