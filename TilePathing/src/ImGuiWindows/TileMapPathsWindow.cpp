#include "TileMapPathsWindow.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

TileMapPathsWindow::TileMapPathsWindow(const bool isOpen) :
    ImGuiWindow("Tile Map Paths", isOpen),
    mPaths(),
    mCurSelected(0)
{}

void TileMapPathsWindow::OnRender()
{
    if (ImGui::BeginListBox("Paths"))
    {
        for (uint32 i = 0; i < std::size(mPaths); ++i)
        {
            char buffer[16] = {};
            snprintf(buffer, sizeof(buffer), "Path %u", i);
            bool isSelected = (mCurSelected == i);
            if (ImGui::Selectable(buffer, &isSelected))
                mCurSelected = i;

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndListBox();
    }

    if (mCurSelected < std::size(mPaths))
    {
        int32 startRow = mPaths[mCurSelected].start.y;
        if (ImGui::InputInt("Start Row", &startRow))
            mPaths[mCurSelected].start.y = startRow;

        int32 startCol = mPaths[mCurSelected].start.x;
        if (ImGui::InputInt("Start Column", &startCol))
            mPaths[mCurSelected].start.x = startCol;

        int32 endRow = mPaths[mCurSelected].end.y;
        if (ImGui::InputInt("End Row", &endRow))
            mPaths[mCurSelected].end.y = endRow;

        int32 endCol = mPaths[mCurSelected].end.x;
        if (ImGui::InputInt("End Column", &endCol))
            mPaths[mCurSelected].end.x = endCol;
    }

    if (ImGui::Button("Add Path"))
    {
        mPaths.push_back({ {0, 0}, {0, 5} });
        mCurSelected = (uint32)std::size(mPaths) - 1;
    }

    if (ImGui::Button("Remove Path") && mCurSelected < std::size(mPaths))
    {
        mPaths.erase(std::begin(mPaths) + mCurSelected);
        mCurSelected = std::max(0, (int32)mCurSelected - 1);
    }
}
