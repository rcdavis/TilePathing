#include "ImGuiWindows/BaseImGuiWindow.h"

#include <imgui.h>

BaseImGuiWindow::BaseImGuiWindow(const std::string& title, bool isOpen) :
    mTitle(title),
    mIsOpen(isOpen)
{}

void BaseImGuiWindow::RenderMenuItem(std::function<void()> onClick)
{
    if (ImGui::MenuItem(std::data(mTitle), nullptr, &mIsOpen) && onClick)
        onClick();
}

void BaseImGuiWindow::Render()
{
    if (mIsOpen)
    {
        if (ImGui::Begin(std::data(mTitle), &mIsOpen))
            OnRender();

        ImGui::End();
    }
}
