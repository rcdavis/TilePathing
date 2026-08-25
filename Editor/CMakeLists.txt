project(Editor)

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
    src/TileMap/TileMapLoader.cpp
    src/TileMap/TilePathing.cpp
    src/TileMap/TileSet.cpp
    src/Utils/MeshUtils.cpp
)

# Copy textures to the build directory
file(GLOB_RECURSE TEXTURE_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/res/textures/*.png)
set(COPIED_TEXTURES "")

foreach(TEXTURE_FILE ${TEXTURE_FILES})
    get_filename_component(TEXTURE_NAME ${TEXTURE_FILE} NAME)
    set(DEST_TEXTURE_FILE ${CMAKE_BINARY_DIR}/Editor/res/textures/${TEXTURE_NAME})

    add_custom_command(
        OUTPUT ${DEST_TEXTURE_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEXTURE_FILE} ${DEST_TEXTURE_FILE}
        DEPENDS ${TEXTURE_FILE}
        COMMENT "Copying texture: ${TEXTURE_NAME}"
    )

    list(APPEND COPIED_TEXTURES ${DEST_TEXTURE_FILE})
endforeach()

add_custom_target(CopyTextures ALL DEPENDS ${COPIED_TEXTURES})
add_dependencies(Editor CopyTextures)

# Copy shaders to the build directory
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/res/shaders/*.vert
    ${CMAKE_SOURCE_DIR}/res/shaders/*.frag
)
set(COPIED_SHADERS "")

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(DEST_SHADER_FILE ${CMAKE_BINARY_DIR}/Editor/res/shaders/${SHADER_NAME})

    add_custom_command(
        OUTPUT ${DEST_SHADER_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILE} ${DEST_SHADER_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Copying shader: ${SHADER_NAME}"
    )

    list(APPEND COPIED_SHADERS ${DEST_SHADER_FILE})
endforeach()

add_custom_target(CopyShaders ALL DEPENDS ${COPIED_SHADERS})
add_dependencies(Editor CopyShaders)

# Create a custom command to compile the tile maps into a binary format
set(TILE_MAP_SOURCES
    ${CMAKE_SOURCE_DIR}/res/tilemaps/SMBMap.tmx
    ${CMAKE_SOURCE_DIR}/res/tilemaps/SMBTiles.tsx
)
set(TEST_MAP_BIN_FILE ${CMAKE_BINARY_DIR}/Editor/res/tilemaps/TestMap.tmbin)

add_custom_command(
    OUTPUT ${TEST_MAP_BIN_FILE}
    COMMAND $<TARGET_FILE:AssetBuilder> tilemaps
        --tilemap
        ${CMAKE_SOURCE_DIR}/res/tilemaps/SMBMap.tmx
        --tileset
        ${CMAKE_SOURCE_DIR}/res/tilemaps/SMBTiles.tsx
        --output
        ${TEST_MAP_BIN_FILE}
    DEPENDS ${TILE_MAP_SOURCES}
    COMMENT "Creating TestMap.tmbin..."
    VERBATIM
)

add_custom_target(CompileTileMaps ALL DEPENDS ${TEST_MAP_BIN_FILE})
add_dependencies(Editor CompileTileMaps)

target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(Editor PRIVATE cxx_std_20)

target_link_libraries(Editor PRIVATE
    pugixml
    Engine
)
