#pragma once

#include <deque>

#include <glm/glm.hpp>

#include "ImGuiWindow.h"

class ConsoleWindow : public ImGuiWindow
{
public:
    struct ConsoleItem
    {
        std::string tag;
        std::string text;
        glm::vec3 color{ 1.0f };
    };

public:
    ConsoleWindow(bool isOpen = false);

    void SetEntryLimit(int limit) { mLimit = limit; }

    void AddLine(ConsoleItem item);
    void Clear() { mItems.clear(); }

    void Render() override;

private:
    std::deque<ConsoleItem> mItems;
    int mLimit = 100;
};
