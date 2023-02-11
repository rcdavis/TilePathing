#include "Application.h"

#include "Log.h"
#include "Input/Input.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLVertexBuffer.h"
#include "OpenGL/GLIndexBuffer.h"
#include "OpenGL/GLShader.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileMapLayer.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileSet.h"

#include "ImGuiWindows/TileMapPropertiesWindow.h"
#include "ImGuiWindows/TileMapPathsWindow.h"

#include "Utils/MeshUtils.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static constexpr uint32 WindowWidth = 1280;
static constexpr uint32 WindowHeight = 738;
//static constexpr uint32 WindowHeight = 720;

Application::Application() :
    mTilePathing(),
    mCamera(0.0f, (f32)WindowWidth, 0.0f, (f32)WindowHeight),
    mImGuiWindows(),
    mTileMap(),
    mLastFrameTime(0.0f),
    mTestTexture(),
    mWindow(nullptr),
    mInitializedImGui(false)
{}

Application::~Application()
{
    Shutdown();
}

void Application::Run()
{
    if (!Init())
    {
        LOG_CRITICAL("Failed to init application.");
        return;
    }

    while (!glfwWindowShouldClose(mWindow))
    {
        const TimeStep time((f32)glfwGetTime());
        const TimeStep timestep = time - mLastFrameTime;
        mLastFrameTime = time;

        glfwPollEvents();

        HandleInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderScene();

        RenderImGuiPanels();

        glfwSwapBuffers(mWindow);
    }

    Shutdown();
}

bool Application::Init()
{
    assert(!mInitializedImGui && "Application is already initialized!");

    glfwSetErrorCallback(GlfwErrorCallback);

    if (!glfwInit())
    {
        LOG_CRITICAL("Failed to initialize GLFW!");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(WindowWidth, WindowHeight, "Tile Pathing", nullptr, nullptr);

    glfwMakeContextCurrent(mWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL("Failed to initialize glad GL loader!");
        return false;
    }

    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    mInitializedImGui = true;

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    mTestTexture = GLTexture::Load("assets/textures/SMB_BlockTiles.png");
    mShader = GLShader::Create("TileMap", "assets/shaders/TileMap.vert", "assets/shaders/TileMap.frag");
    mTileMap = TileMap::Load("assets/tilemaps/SMBMap.tmx");

    mColorShader = GLShader::Create("ColoredTile", "assets/shaders/ColoredTile.vert", "assets/shaders/ColoredTile.frag");

    mTilePathing.SetTileMap(mTileMap);

    mVAO = MeshUtils::CreateTileMapMesh(mTileMap);
    mColoredRectVao = MeshUtils::CreateColoredTileMesh(mTileMap);

    mImGuiWindows.push_back(CreateRef<TileMapPropertiesWindow>());
    mImGuiWindows.push_back(CreateRef<TileMapPathsWindow>(true));

    return true;
}

void Application::Shutdown()
{
    mTestTexture = nullptr;
    mShader = nullptr;
    mTileMap = nullptr;
    mVAO = nullptr;

    mColoredRectVao = nullptr;
    mColorShader = nullptr;

    if (mInitializedImGui)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        mInitializedImGui = false;
    }

    glfwTerminate();

    mWindow = nullptr;
}

void Application::RenderScene()
{
    mTestTexture->Bind();

    mShader->Bind();
    mShader->SetMat4("u_ViewProjection", mCamera.GetViewProjection());

    mVAO->Bind();

    Render(mVAO);

    RenderTilePaths();
}

void Application::RenderTilePaths()
{
    glEnable(GL_BLEND);

    mColoredRectVao->Bind();
    mColorShader->Bind();
    mColorShader->SetMat4("u_ViewProjection", mCamera.GetViewProjection());

    auto tileMapPropertiesWindow = DynamicCastRef<TileMapPropertiesWindow>(mImGuiWindows[0]);
    auto tileMapPathsWindow = DynamicCastRef<TileMapPathsWindow>(mImGuiWindows[1]);

    for (const auto& p : tileMapPathsWindow->GetPaths())
    {
        const auto path = mTilePathing.FindPath(p.start, p.end);
        for (const glm::uvec2 cell : path)
        {
            glm::vec4 color;
            if (cell == p.start)
                color = tileMapPropertiesWindow->GetStartColor();
            else if (cell == p.end)
                color = tileMapPropertiesWindow->GetEndColor();
            else
                color = tileMapPropertiesWindow->GetPathColor();

            mColorShader->SetMat4("u_Transform", GetTileTransform(cell));
            mColorShader->SetFloat4("u_Color", color);

            Render(mColoredRectVao);
        }

        for (const glm::uvec2 cell : mTilePathing.GetVisitedCoords())
        {
            mColorShader->SetMat4("u_Transform", GetTileTransform(cell));
            mColorShader->SetFloat4("u_Color", tileMapPropertiesWindow->GetCheckedColor());

            Render(mColoredRectVao);
        }
    }

    glDisable(GL_BLEND);
}

void Application::RenderImGuiPanels()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Windows"))
        {
            for (auto& window : mImGuiWindows)
                window->RenderMenuItem();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    for (auto& window : mImGuiWindows)
        window->Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::HandleInput()
{
    /*if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
    {
        mStartCoords = GetTileCoords(Input::GetMousePosition());
    }
    else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
    {
        mEndCoords = GetTileCoords(Input::GetMousePosition());
    }*/
}

glm::mat4 Application::GetTileTransform(glm::uvec2 coords)
{
    const uint32 tileWidth = mTileMap->GetTileWidth();
    const uint32 tileHeight = mTileMap->GetTileHeight();
    const uint32 numTilesWidth = mTileMap->GetWidth();
    const uint32 numTilesHeight = mTileMap->GetHeight();

    const uint32 xPos = coords.x * tileWidth;
    const int32 yPos = -((int32)coords.y * (int32)tileHeight);

    return glm::translate(glm::mat4(1.0f), glm::vec3((f32)xPos, (f32)yPos, 0.5f));
}

glm::uvec2 Application::GetTileCoords(glm::uvec2 mousePos)
{
    const uint32 tileWidth = mTileMap->GetTileWidth();
    const uint32 tileHeight = mTileMap->GetTileHeight();

    return { mousePos.x / tileWidth, (mousePos.y - tileHeight) / tileHeight };
}

void Application::Render(const Ref<GLVertexArray>& vao)
{
    const uint32 count = vao->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
}

void Application::GlfwErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}
