#include "Application.h"

#include "Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    assert(!mInitialized && "Application is already initialized!");

    mConsoleWindow.AddLine({ "INFO", "Starting app...", glm::vec3 { 0.0f, 1.0f, 0.0f } });
    mConsoleWindow.AddLine({ "ERROR", "Test Error", glm::vec3 { 1.0f, 0.0f, 0.0f } });
    for (int i = 1; i <= 98; ++i)
    {
        char message[64] = {};
        std::sprintf(message, "Debug message %i", i);
        mConsoleWindow.AddLine({ "DEBUG", message, glm::vec3 { 1.0f } });
    }

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    mInitialized = true;

    return true;
}

void Application::Shutdown()
{
    if (mInitialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();

    mWindow = nullptr;
    mInitialized = false;
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
            if (ImGui::MenuItem("Tile Map Properties", nullptr, &mIsTileMapPropertiesWindowOpen))
                mConsoleWindow.AddLine({ "INFO", "Opened Tile Map Properties", glm::vec3 { 0.0f, 1.0f, 0.0f } });

            bool isConsoleWindowOpen = mConsoleWindow.IsOpen();
            if (ImGui::MenuItem("Console", nullptr, &isConsoleWindowOpen))
                mConsoleWindow.Open(isConsoleWindowOpen);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (mIsTileMapPropertiesWindowOpen)
    {
        ImGui::Begin("Tile Map Properties", &mIsTileMapPropertiesWindowOpen);
        ImGui::ColorEdit3("Path Color", glm::value_ptr(mPathColor));
        ImGui::ColorEdit3("Checked Color", glm::value_ptr(mCheckedColor));
        ImGui::End();
    }

    mConsoleWindow.Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::GlfwErrorCallback(int error, const char* description)
{
    LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}
