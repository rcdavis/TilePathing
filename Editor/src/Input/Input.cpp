#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>

TimeStep Input::s_CurTime = 0.0f;

void Input::Poll(TimeStep ts)
{
    s_CurTime += ts;
}

bool Input::IsKeyPressed(const KeyCode keycode)
{
    auto window = Application::Get().GetWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS;
}

bool Input::IsKeyReleased(const KeyCode keycode)
{
    auto window = Application::Get().GetWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_RELEASE;
}

bool Input::IsKeyDown(const KeyCode keycode, const TimeStep timeDiff)
{
    if (s_CurTime <= timeDiff)
        return false;

    auto window = Application::Get().GetWindow();
    auto state = glfwGetKey(window, keycode);
    if (state == GLFW_PRESS || state == GLFW_REPEAT)
    {
        s_CurTime = 0.0f;
        return true;
    }

    return false;
}

bool Input::IsMouseButtonPressed(const MouseCode button)
{
    auto window = Application::Get().GetWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
    auto window = Application::Get().GetWindow();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return { (f32)x, (f32)y };
}

f32 Input::GetMouseX()
{
    return GetMousePosition().x;
}

f32 Input::GetMouseY()
{
    return GetMousePosition().y;
}
