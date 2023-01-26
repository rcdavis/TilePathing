#include "Application.h"

#include "Log.h"
#include "OpenGL/GLTexture.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    mWindow = glfwCreateWindow(1280, 720, "Tile Pathing", nullptr, nullptr);

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
    glEnable(GL_CULL_FACE);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    mTestTexture = GLTexture::Load("assets/textures/SMB_BlockTiles.png");

    return true;
}

void Application::Shutdown()
{
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

void Application::GlfwErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}
