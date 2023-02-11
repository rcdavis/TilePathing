#pragma once

#include "Core.h"
#include "TimeStep.h"
#include "Camera.h"

#include "ImGuiWindows/TileMapPropertiesWindow.h"
#include "TileMap/TilePathing.h"

#include <glm/glm.hpp>

struct GLFWwindow;
class GLTexture;
class GLVertexArray;
class GLShader;
class TileMap;

class Application
{
public:
    struct Vertex
    {
        glm::vec3 position{ 0.0f };
        glm::vec2 texCoord{ 0.0f };
    };

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

    void RenderScene();
    void RenderImGuiPanels();

    std::vector<Vertex> CreateTileMapVertices();
    void CreateTileMapMesh();

    void CreateColoredTileMesh();
    void RenderTilePaths();

    void HandleInput();

    glm::mat4 GetTileTransform(glm::uvec2 coords);
    glm::uvec2 GetTileCoords(glm::uvec2 mousePos);

    void Render(const Ref<GLVertexArray>& vao);

    static void GlfwErrorCallback(int error, const char* description);

private:
    TileMapPropertiesWindow mTileMapPropertiesWindow;
    TilePathing mTilePathing;
    Camera mCamera;
    TimeStep mLastFrameTime;

    glm::uvec2 mStartCoords{ 65, 28 };
    glm::uvec2 mEndCoords{ 58, 21 };

    Ref<GLVertexArray> mVAO;
    Ref<GLTexture> mTestTexture;
    Ref<TileMap> mTileMap;
    Ref<GLShader> mShader;

    Ref<GLVertexArray> mColoredRectVao;
    Ref<GLShader> mColorShader;

    GLFWwindow* mWindow = nullptr;

    bool mInitializedImGui = false;
};
