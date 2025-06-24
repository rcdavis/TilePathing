#pragma once

#include "Core.h"
#include "ImGuiWindows/BaseImGuiWindow.h"

#include <vector>

#include <glm/glm.hpp>

class TileMapPathsWindow : public BaseImGuiWindow
{
public:
    struct Path
    {
        glm::uvec2 start{ 0, 0 };
        glm::uvec2 end{ 0, 0 };
    };

public:
    TileMapPathsWindow(const bool isOpen = false);

    const std::vector<Path>& GetPaths() const { return mPaths; }

    void OnRender() override;

private:
    std::vector<Path> mPaths;
    uint32 mCurSelected = 0;
};
