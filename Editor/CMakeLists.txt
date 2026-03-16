project(Editor)

include(FetchContent)

FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.15
)
FetchContent_MakeAvailable(pugixml)

add_executable(Editor
    src/Application.cpp
    src/Camera.cpp
    src/Main.cpp
    src/Property.cpp
    src/ImGuiWindows/CharacterWindow.cpp
    src/ImGuiWindows/ConsoleWindow.cpp
    src/ImGuiWindows/ContentBrowserWindow.cpp
    src/ImGuiWindows/BaseImGuiWindow.cpp
    src/ImGuiWindows/TileMapPathsWindow.cpp
    src/ImGuiWindows/TileMapPropertiesWindow.cpp
    src/Input/Input.cpp
    src/TileMap/TileLayer.cpp
    src/TileMap/TileMap.cpp
    src/TileMap/TilePathing.cpp
    src/TileMap/TileSet.cpp
    src/Utils/MeshUtils.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_BINARY_DIR}/Editor)

target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Editor PRIVATE
    pugixml
    Engine
)
