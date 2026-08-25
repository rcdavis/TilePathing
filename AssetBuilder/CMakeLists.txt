project(AssetBuilder)

add_executable(AssetBuilder
    src/AssetBuilder.cpp
    src/Main.cpp
)

target_include_directories(AssetBuilder PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(AssetBuilder PRIVATE
    CLI11::CLI11
    pugixml
)
