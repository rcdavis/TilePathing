#pragma once

#include "Core.h"

#include <glm/glm.hpp>

#include "ImGuiWindow.h"

class TileMapPropertiesWindow : public ImGuiWindow
{
public:
    TileMapPropertiesWindow(bool isOpen = false);

    glm::vec4 GetStartColor() const { return mStartColor; }
    void SetStartColor(glm::vec4 color) { mStartColor = color; }

    glm::vec4 GetEndColor() const { return mEndColor; }
    void SetEndColor(glm::vec4 color) { mEndColor = color; }

    glm::vec4 GetPathColor() const { return mPathColor; }
    void SetPathColor(glm::vec4 color) { mPathColor = color; }

    glm::vec4 GetCheckedColor() const { return mCheckedColor; }
    void SetCheckedColor(glm::vec4 color) { mCheckedColor = color; }

    int32 GetMaxSteps() const { return mMaxSteps; }
    void SetMaxSteps(int32 maxSteps) { mMaxSteps = maxSteps; }

    bool GetShowVisitedTiles() const { return mShowVisitedTiles; }
    void SetShowVisitedTiles(const bool b) { mShowVisitedTiles = b; }

    void OnRender() override;

private:
    glm::vec4 mStartColor{ 0.0f, 0.0f, 1.0f, 0.5f };
    glm::vec4 mEndColor{ 1.0f, 0.0f, 0.0f, 0.5f };
    glm::vec4 mPathColor{ 1.0f, 1.0f, 1.0f, 0.5f };
    glm::vec4 mCheckedColor{ 0.5f, 0.5f, 0.5f, 0.5f };

    int32 mMaxSteps = -1;

    bool mShowVisitedTiles = false;
};
