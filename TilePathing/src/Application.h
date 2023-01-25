#pragma once

#include "Core.h"

#include <glm/glm.hpp>

struct GLFWwindow;

class Application
{
public:
    Application() = default;
    ~Application();

    void Run();

private:
    bool Init();
    void Shutdown();

    void RenderImGuiPanels();

    static void GlfwErrorCallback(int error, const char* description);

private:
    GLFWwindow* mWindow = nullptr;

    glm::vec4 mPathColor{ 1.0f, 1.0f, 1.0f, 0.5f };
    glm::vec4 mCheckedColor{ 0.5f, 0.5f, 0.5f, 0.5f };

    bool mInitialized = false;
    bool mIsTileMapPropertiesWindowOpen = false;
};
