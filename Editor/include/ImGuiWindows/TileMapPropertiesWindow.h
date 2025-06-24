#pragma once

#include "Core.h"

#include <glm/glm.hpp>

#include "ImGuiWindows/BaseImGuiWindow.h"

class TileMapPropertiesWindow : public BaseImGuiWindow
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

    glm::vec4 GetSelectionColor() const { return mSelectionColor; }
    void SetSelectionColor(glm::vec4 color) { mSelectionColor = color; }

    glm::vec4 GetMovementZoneColor() const { return mMovementZoneColor; }
    void SetMovementZoneColor(glm::vec4 color) { mMovementZoneColor = color; }

    bool GetShowVisitedTiles() const { return mShowVisitedTiles; }
    void SetShowVisitedTiles(const bool b) { mShowVisitedTiles = b; }

    void OnRender() override;

private:
    glm::vec4 mStartColor{ 0.0f, 0.0f, 1.0f, 0.5f };
    glm::vec4 mEndColor{ 1.0f, 0.0f, 0.0f, 0.5f };
    glm::vec4 mPathColor{ 0.0f, 1.0f, 1.0f, 0.5f };
    glm::vec4 mCheckedColor{ 0.5f, 0.5f, 0.5f, 0.5f };

    glm::vec4 mSelectionColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4 mMovementZoneColor{ 0.0f, 0.0f, 1.0f, 0.5f };

    bool mShowVisitedTiles = false;
};
