#include "Application.h"

#include "Log.h"
#include "Character.h"
#include "Input/Input.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLVertexBuffer.h"
#include "OpenGL/GLIndexBuffer.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLFramebuffer.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileMapLayer.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileSet.h"

#include "ImGuiWindows/TileMapPropertiesWindow.h"
#include "ImGuiWindows/TileMapPathsWindow.h"
#include "ImGuiWindows/ContentBrowserWindow.h"

#include "Utils/MeshUtils.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static constexpr uint32 WindowWidth = 1280;
static constexpr uint32 WindowHeight = 720;

Application::Application() :
    mTilePathing(),
    mCamera(0.0f, (f32)WindowWidth, 0.0f, (f32)WindowHeight),
    mImGuiWindows(),
    mCharacters(),
    mPlayer(),
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

        Input::Poll(timestep);

        glfwPollEvents();

        HandleInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Render();

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

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

    mImGuiWindows.push_back(CreateRef<TileMapPropertiesWindow>(true));
    mImGuiWindows.push_back(CreateRef<TileMapPathsWindow>(true));
    mImGuiWindows.push_back(CreateRef<ContentBrowserWindow>(true));

    mPlayer = CreateRef<Character>();
    mPlayer->SetTexture(GLTexture::Load("assets/textures/FileIcon.png"));
    mPlayer->SetVertexArray(MeshUtils::CreateColoredTileMesh(mTileMap));
    mCharacters.push_back(mPlayer);

    FramebufferSpecs specs;
    specs.attachments = {
        FramebufferTextureFormat::RGBA8,
        FramebufferTextureFormat::Depth
    };
    specs.width = WindowWidth;
    specs.height = WindowHeight;
    mFramebuffer = GLFramebuffer::Create(specs);

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

    mFramebuffer = nullptr;

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
    mFramebuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mTestTexture->Bind();

    mShader->Bind();
    mShader->SetMat4("u_ViewProjection", mCamera.GetViewProjection());
    mShader->SetMat4("u_Transform", glm::mat4(1.0f));

    mVAO->Bind();
    Render(mVAO);

    for (const auto& c : mCharacters)
    {
        c->GetVertexArray()->Bind();
        c->GetTexture()->Bind();
        mShader->SetMat4("u_Transform", GetTileTransform(c->GetTileCoords()));
        Render(c->GetVertexArray());
    }

    RenderTilePaths();

    auto mousePos = ImGui::GetMousePos();
    mousePos.x -= mViewportBounds[0].x;
    mousePos.y -= mViewportBounds[0].y;

    const glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
    mousePos.y = viewportSize.y - mousePos.y;
    mMousePos = glm::vec2((f32)mousePos.x, (f32)mousePos.y);

    mFramebuffer->Unbind();
}

void Application::RenderTilePaths()
{
    auto tileMapPropertiesWindow = DynamicCastRef<TileMapPropertiesWindow>(mImGuiWindows[0]);
    auto tileMapPathsWindow = DynamicCastRef<TileMapPathsWindow>(mImGuiWindows[1]);
    if (!tileMapPropertiesWindow || !tileMapPathsWindow)
        return;

    glEnable(GL_BLEND);

    mColoredRectVao->Bind();
    mColorShader->Bind();
    mColorShader->SetMat4("u_ViewProjection", mCamera.GetViewProjection());

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

        if (tileMapPropertiesWindow->GetShowVisitedTiles())
        {
            for (const glm::uvec2 cell : mTilePathing.GetVisitedCoords())
            {
                mColorShader->SetMat4("u_Transform", GetTileTransform(cell));
                mColorShader->SetFloat4("u_Color", tileMapPropertiesWindow->GetCheckedColor());

                Render(mColoredRectVao);
            }
        }
    }

    glDisable(GL_BLEND);
}

void Application::Render()
{
    RenderScene();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool dockspaceOpen = true;
    static constexpr bool optFullscreen = true;
    static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if constexpr (optFullscreen)
    {
        const ImGuiViewport* const viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if constexpr (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
    ImGui::PopStyleVar();

    if constexpr (optFullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockspaceFlags);

    RenderMainMenu();

    for (auto& window : mImGuiWindows)
        window->Render();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");

    const auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    const auto viewportOffset = ImGui::GetWindowPos();
    mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    const auto viewportPanelSize = ImGui::GetContentRegionAvail();
    mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    mViewportClickable = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();

    const uint64 texId = (uint64)mFramebuffer->GetColorAttachment();
    ImGui::Image((ImTextureID)texId, viewportPanelSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* const backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void Application::RenderMainMenu()
{
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
}

void Application::HandleInput()
{
    if (mViewportClickable && mPlayer && mTileMap)
    {
        constexpr TimeStep ts(0.1f);
        const auto coords = mPlayer->GetTileCoords();

        if (Input::IsKeyDown(Key::Left, ts))
        {
            if (coords.x > 0)
                mPlayer->MoveLeft(1);
        }
        else if (Input::IsKeyDown(Key::Right, ts))
        {
            if (coords.x < mTileMap->GetWidth() - 1)
                mPlayer->MoveRight(1);
        }

        if (Input::IsKeyDown(Key::Up, ts))
        {
            if (coords.y > 0)
                mPlayer->MoveUp(1);
        }
        else if (Input::IsKeyDown(Key::Down, ts))
        {
            if (coords.y < mTileMap->GetHeight() - 1)
                mPlayer->MoveDown(1);
        }
    }
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
