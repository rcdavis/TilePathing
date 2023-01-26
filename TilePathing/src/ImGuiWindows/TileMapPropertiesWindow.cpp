#include "TileMapPropertiesWindow.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TileMapPropertiesWindow::TileMapPropertiesWindow(bool isOpen) :
    ImGuiWindow("Tile Map Properties", isOpen)
{}

void TileMapPropertiesWindow::Render()
{
    if (mIsOpen)
    {
        ImGui::Begin(std::data(mTitle), &mIsOpen);
        ImGui::ColorEdit3("Path Color", glm::value_ptr(mPathColor));
        ImGui::ColorEdit3("Checked Color", glm::value_ptr(mCheckedColor));
        ImGui::End();
    }
}
