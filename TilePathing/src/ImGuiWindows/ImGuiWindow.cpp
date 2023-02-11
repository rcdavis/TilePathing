#include "ImGuiWindow.h"

#include <imgui.h>

ImGuiWindow::ImGuiWindow(const std::string& title, bool isOpen) :
    mTitle(title),
    mIsOpen(false)
{}

void ImGuiWindow::RenderMenuItem(std::function<void()> onClick)
{
    if (ImGui::MenuItem(std::data(mTitle), nullptr, &mIsOpen) && onClick)
        onClick();
}

void ImGuiWindow::Render()
{
    if (mIsOpen)
    {
        if (ImGui::Begin(std::data(mTitle), &mIsOpen))
            OnRender();

        ImGui::End();
    }
}
