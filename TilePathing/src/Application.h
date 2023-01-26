#pragma once

#include "Core.h"

#include "ImGuiWindows/TileMapPropertiesWindow.h"

struct GLFWwindow;
class GLTexture;

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
    Ref<GLTexture> mTestTexture;

    GLFWwindow* mWindow = nullptr;

    bool mInitializedImGui = false;
};
