#include "Application.h"

#include "Log.h"
#include "OpenGL/GLTexture.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLVertexBuffer.h"
#include "OpenGL/GLIndexBuffer.h"
#include "OpenGL/GLShader.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileMapLayer.h"
#include "TileMap/TileLayer.h"
#include "TileMap/TileSet.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>

static constexpr uint32 WindowWidth = 1280;
static constexpr uint32 WindowHeight = 738;
//static constexpr uint32 WindowHeight = 720;

static constexpr std::array startPaths = {
    glm::uvec2 { 65, 28 }
};

static constexpr std::array endPaths = {
    glm::uvec2 { 58, 21 }
};

static_assert(std::size(startPaths) == std::size(endPaths));

Application::Application() :
    mTileMapPropertiesWindow(),
    mCamera(0.0f, (f32)WindowWidth, 0.0f, (f32)WindowHeight),
    mTileMap(),
    mLastFrameTime(0.0f),
    mTileMapTransform(1.0f),
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

    CreateTileMapMesh();
    CreateColoredTileMesh();

    mTileMapTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));

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

    const uint32 count = mVAO->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);

    RenderTilePaths();
}

void Application::RenderTilePaths()
{
    glEnable(GL_BLEND);

    mColoredRectVao->Bind();
    mColorShader->Bind();
    mColorShader->SetMat4("u_ViewProjection", mCamera.GetViewProjection());

    for (int i = 0; i < std::size(startPaths); ++i)
    {
        const auto path = mTilePathing.FindPath(startPaths[i], endPaths[i]);
        for (const glm::uvec2 cell : path)
        {
            glm::vec4 color;
            if (cell == startPaths[i])
                color = mTileMapPropertiesWindow.GetStartColor();
            else if (cell == endPaths[i])
                color = mTileMapPropertiesWindow.GetEndColor();
            else
                color = mTileMapPropertiesWindow.GetPathColor();

            mColorShader->SetMat4("u_Transform", GetTileTransform(cell));
            mColorShader->SetFloat4("u_Color", color);

            const uint32 count = mColoredRectVao->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
        }

        for (const glm::uvec2 cell : mTilePathing.GetVisitedCoords())
        {
            mColorShader->SetMat4("u_Transform", GetTileTransform(cell));
            mColorShader->SetFloat4("u_Color", mTileMapPropertiesWindow.GetCheckedColor());

            const uint32 count = mColoredRectVao->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
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
            mTileMapPropertiesWindow.RenderMenuItem();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    mTileMapPropertiesWindow.Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::vector<Application::Vertex> Application::CreateTileMapVertices()
{
    assert(mTileMap && "Passing in a null tile map");
    assert(!std::empty(mTileMap->GetTileSets()) && "Tile map does not have a tile set");

    std::vector<Application::Vertex> vertices;

    for (const Ref<TileMapLayer>& layer : mTileMap->GetLayers())
    {
        if (layer->GetType() != TileMapLayer::Type::Tile)
            continue;

        const auto& tileLayer = DynamicCastRef<TileLayer>(layer);
        const auto& tiles = tileLayer->GetTiles();
        for (uint32 i = 0; i < std::size(tiles); ++i)
        {
            const auto& tile = tiles[i];
            Ref<TileSet> tileSet;
            for (const auto& ts : mTileMap->GetTileSets())
            {
                if (tile.mId >= ts->GetFirstGid())
                {
                    tileSet = ts;
                    break;
                }
            }

            assert(tileSet && "Tile Layer does not have a tile set");

            const uint32 tileWidth = tileSet->GetTileWidth();
            const uint32 tileHeight = tileSet->GetTileHeight();
            const uint32 numTilesWidth = tileLayer->GetWidth();
            const uint32 numTilesHeight = tileLayer->GetHeight();

            const uint32 xPos = ((i % numTilesWidth) * tileWidth);
            const uint32 yPos = (numTilesHeight * tileHeight) - ((i / numTilesWidth) * tileHeight);

            const std::array<glm::vec4, 4> vertPositions = {
                glm::vec4 { xPos, yPos, 0.0f, 1.0f },
                glm::vec4 { xPos + tileWidth, yPos, 0.0f, 1.0f },
                glm::vec4 { xPos + tileWidth, yPos - tileHeight, 0.0f, 1.0f },
                glm::vec4 { xPos, yPos - tileHeight, 0.0f, 1.0f }
            };

            const std::array<glm::vec2, 4> texCoords = tileSet->GetTexCoords(tile.mId);

            for (int i = 0; i < 4; ++i)
                vertices.push_back({ vertPositions[i], texCoords[i] });
        }
    }

    return vertices;
}

void Application::CreateTileMapMesh()
{
    const auto vertices = CreateTileMapVertices();

    mVAO = GLVertexArray::Create();
    mVAO->Bind();

    auto vb = GLVertexBuffer::Create((uint32)std::size(vertices) * sizeof(Vertex));
    vb->SetData(std::data(vertices), (uint32)std::size(vertices) * sizeof(Vertex));
    vb->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
        });
    mVAO->AddVertexBuffer(vb);

    std::vector<uint16> quadIndices(std::size(vertices) * 6);

    uint16 offset = 0;
    for (size_t i = 0; i < std::size(quadIndices); i += 6)
    {
        quadIndices[i] = offset;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset;

        offset += 4;
    }

    auto quadIB = GLIndexBuffer::Create(std::data(quadIndices), (uint32)std::size(quadIndices));
    mVAO->SetIndexBuffer(quadIB);

    mVAO->Unbind();
}

void Application::CreateColoredTileMesh()
{
    assert(mTileMap && "Passing in a null tile map");

    mColoredRectVao = GLVertexArray::Create();
    mColoredRectVao->Bind();

    const uint32 numTilesHeight = mTileMap->GetHeight();
    const uint32 tileWidth = mTileMap->GetTileWidth();
    const uint32 tileHeight = mTileMap->GetTileHeight();

    constexpr f32 xPos = 0.0f;
    const uint32 yPos = (numTilesHeight * tileHeight);

    const std::array<glm::vec4, 4> vertPositions = {
        glm::vec4 { xPos, yPos, 0.0f, 1.0f },
        glm::vec4 { xPos + tileWidth, yPos, 0.0f, 1.0f },
        glm::vec4 { xPos + tileWidth, yPos - tileHeight, 0.0f, 1.0f },
        glm::vec4 { xPos, yPos - tileHeight, 0.0f, 1.0f }
    };

    const std::array<Vertex, 4> vertices = {
        Vertex { vertPositions[0], glm::vec2(0.0f) },
        Vertex { vertPositions[1], glm::vec2(0.0f) },
        Vertex { vertPositions[2], glm::vec2(0.0f) },
        Vertex { vertPositions[3], glm::vec2(0.0f) }
    };

    auto vb = GLVertexBuffer::Create((uint32)std::size(vertices) * sizeof(Vertex));
    vb->SetData(std::data(vertices), (uint32)std::size(vertices) * sizeof(Vertex));
    vb->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
        });
    mColoredRectVao->AddVertexBuffer(vb);

    constexpr std::array<uint16, 6> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto quadIB = GLIndexBuffer::Create(std::data(indices), (uint32)std::size(indices));
    mColoredRectVao->SetIndexBuffer(quadIB);

    mColoredRectVao->Unbind();
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

void Application::GlfwErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}
