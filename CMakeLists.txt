cmake_minimum_required(VERSION 3.28)

project(TilePathing LANGUAGES CXX)

include(cmake/dependencies.cmake)

add_subdirectory(AssetBuilder)
add_subdirectory(Engine)
add_subdirectory(Editor)
