
project "Benchmarking"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir (outputTargetDir)
    objdir (outputObjDir)

    files { "src/**.h", "src/**.hpp", "src/**.cpp" }

    includedirs {
        "src",
        "%{IncludeDir.benchmark}"
    }

    links {
        "%{Library.benchmark}"
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "BENCHMARK_STATIC_DEFINE"
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
