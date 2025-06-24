#include "ImGuiWindows/ConsoleWindow.h"

#include <imgui.h>

ConsoleWindow::ConsoleWindow(bool isOpen) :
    BaseImGuiWindow("Console", isOpen)
{}

void ConsoleWindow::AddLine(ConsoleItem item)
{
    if (std::size(mItems) == mLimit)
        mItems.pop_front();

    mItems.push_back(item);
}

void ConsoleWindow::OnRender()
{
    if (ImGui::BeginChild("ConsoleLines"))
    {
        for (const ConsoleItem& item : mItems)
        {
            const ImVec4 color(item.color.r, item.color.g, item.color.b, 1.0f);
            ImGui::TextColored(color, "[%s] %s", std::data(item.tag), std::data(item.text));
        }
    }
    ImGui::EndChild();
}
