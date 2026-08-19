project(AssetBuilder)

add_executable(AssetBuilder
    src/Main.cpp
)

target_link_libraries(AssetBuilder PRIVATE
    CLI11::CLI11
)
