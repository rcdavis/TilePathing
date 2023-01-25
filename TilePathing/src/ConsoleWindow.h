#pragma once

#include <deque>
#include <string>

#include <glm/glm.hpp>

class ConsoleWindow
{
public:
    struct ConsoleItem
    {
        std::string tag;
        std::string text;
        glm::vec3 color{ 1.0f };
    };

public:
    ConsoleWindow() = default;

    void SetEntryLimit(int limit) { mLimit = limit; }

    bool IsOpen() const { return mIsOpen; }
    void Open(bool isOpen) { mIsOpen = isOpen; }

    void AddLine(ConsoleItem item);
    void Clear() { mItems.clear(); }

    void Render();

private:
    std::deque<ConsoleItem> mItems;
    int mLimit = 100;
    bool mIsOpen = false;
};
