project(Engine)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

set(SPDLOG_HEADER_ONLY ON CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)

add_library(Engine STATIC
    ImGui/src/imgui.cpp
    ImGui/src/imgui_draw.cpp
    ImGui/src/imgui_widgets.cpp
    ImGui/src/imgui_tables.cpp
    ImGui/src/imgui_demo.cpp
    ImGui/src/imgui_impl_glfw.cpp
    ImGui/src/imgui_impl_opengl3.cpp
    ImGuiFileDialog/src/ImGuiFileDialog.cpp
    glad/src/glad.c
    stb/src/stb_image.c
    src/OpenGL/GLFramebuffer.cpp
    src/OpenGL/GLIndexBuffer.cpp
    src/OpenGL/GLShader.cpp
    src/OpenGL/GLTexture.cpp
    src/OpenGL/GLVertexArray.cpp
    src/OpenGL/GLVertexBuffer.cpp
    src/Utils/FileUtils.cpp
    src/Utils/StringUtils.cpp
    src/Log.cpp
)

target_compile_definitions(Engine PUBLIC
    GLFW_INCLUDE_NONE
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Engine PUBLIC
        DEBUG
        LOGGING_ENABLED
        ASSERTS_ENABLED
    )
endif()

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glm/include
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/include
)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

target_link_libraries(Engine PUBLIC
    glfw
    spdlog::spdlog_header_only
    OpenGL::GL
)
