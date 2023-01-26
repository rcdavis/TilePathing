#pragma once

#include "Core.h"

#include "ImGuiWindows/TileMapPropertiesWindow.h"

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
    TileMapPropertiesWindow mTileMapPropertiesWindow;

    GLFWwindow* mWindow = nullptr;

    bool mInitializedImGui = false;
};
