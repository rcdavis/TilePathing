
project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir (outputTargetDir)
    objdir (outputObjDir)

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "%{IncludeDir.Spdlog}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.pugixml}"
    }

    links {
        "%{Library.glfw}",
        "%{Library.glad}",
        "%{Library.pugixml}",
        "opengl32.lib",
        "ImGui"
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "STB_IMAGE_IMPLEMENTATION"
    }

    filter { "configurations:Debug" }
        symbols "On"
        defines { "LOGGING_ENABLED" }

    filter { "configurations:Release" }
        symbols "On"
        optimize "On"
        defines { "LOGGING_ENABLED" }

    filter { "configurations:Dist" }
        optimize "On"
