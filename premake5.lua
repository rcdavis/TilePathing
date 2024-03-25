
include "Dependencies.lua"

workspace "TilePathing"
    startproject "Editor"
    configurations { "Debug", "Release", "Dist" }
    platforms { "Win64" }
    flags { "MultiProcessorCompile" }

    filter { "platforms:Win64" }
        system "windows"
        architecture "x86_64"

    filter { "system:windows" }
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }
        systemversion "latest"

outputTargetDir = "Build/%{cfg.platform}-%{cfg.buildcfg}/Bin"
outputObjDir = "Build/%{cfg.platform}-%{cfg.buildcfg}/Obj"

include "Vendor/ImGui"
--include "TilePathing"
include "Editor"
include "Benchmarking"
