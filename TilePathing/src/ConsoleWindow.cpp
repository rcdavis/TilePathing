#include "ConsoleWindow.h"

#include <imgui.h>

void ConsoleWindow::AddLine(ConsoleItem item)
{
    if (std::size(mItems) == mLimit)
        mItems.pop_front();

    mItems.push_back(item);
}

void ConsoleWindow::Render()
{
    if (mIsOpen)
    {
        ImGui::Begin("Console", &mIsOpen);

        if (ImGui::BeginChild("ConsoleLines"))
        {
            for (const ConsoleItem& item : mItems)
            {
                const ImVec4 color(item.color.r, item.color.g, item.color.b, 1.0f);
                ImGui::TextColored(color, "[%s] %s", std::data(item.tag), std::data(item.text));
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}
