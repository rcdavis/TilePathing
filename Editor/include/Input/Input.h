#pragma once

#include "Input/KeyCodes.h"
#include "Input/MouseButtonCodes.h"

#include "TimeStep.h"

#include <glm/glm.hpp>

class Input
{
public:
    Input() = delete;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    static void Poll(TimeStep ts);

    static bool IsKeyPressed(const KeyCode keycode);
    static bool IsKeyReleased(const KeyCode keycode);
    static bool IsKeyDown(const KeyCode keycode, const TimeStep timeDiff = 0.0f);
    static bool IsMouseButtonPressed(const MouseCode button);

    static glm::vec2 GetMousePosition();
    static f32 GetMouseX();
    static f32 GetMouseY();

private:
    static TimeStep s_CurTime;
};
