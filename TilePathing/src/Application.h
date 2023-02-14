#pragma once

#include "Core.h"
#include "TimeStep.h"
#include "Camera.h"

#include "TileMap/TilePathing.h"

#include <array>

#include <glm/glm.hpp>

struct GLFWwindow;
class GLTexture;
class GLVertexArray;
class GLFramebuffer;
class GLShader;
class TileMap;
class ImGuiWindow;
class Character;

class Application
{
public:
    Application();
    ~Application();

    void Run();

    static Application& Get()
    {
        static Application app;
        return app;
    }

    GLFWwindow* GetWindow() { return mWindow; }

private:
    bool Init();
    void Shutdown();

    void Render();
    void RenderScene();
    void RenderMainMenu();
    void RenderTilePaths();

    void HandleInput();

    glm::mat4 GetTileTransform(glm::uvec2 coords);
    glm::uvec2 GetTileCoords(glm::uvec2 mousePos);

    void Render(const Ref<GLVertexArray>& vao);

    static void GlfwErrorCallback(int error, const char* description);

private:
    TilePathing mTilePathing;
    Camera mCamera;

    std::vector<Ref<ImGuiWindow>> mImGuiWindows;
    Ref<Character> mPlayer;

    glm::uvec2 mStartPos{ 0, 0 };

    glm::vec2 mMousePos{ 0.0f, 0.0f };
    glm::vec2 mViewportSize{ 0.0f, 0.0f };
    std::array<glm::vec2, 2> mViewportBounds = {
        glm::vec2 { 0.0f, 0.0f },
        glm::vec2 { 0.0f, 0.0f }
    };

    TimeStep mLastFrameTime;

    Ref<GLFramebuffer> mFramebuffer;

    Ref<GLVertexArray> mVAO;
    Ref<GLTexture> mTestTexture;
    Ref<TileMap> mTileMap;
    Ref<GLShader> mShader;

    Ref<GLVertexArray> mColoredRectVao;
    Ref<GLShader> mColorShader;

    GLFWwindow* mWindow = nullptr;

    bool mInitializedImGui = false;
    bool mViewportClickable = false;
};
