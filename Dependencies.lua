
IncludeDir = {}
IncludeDir["Spdlog"] = "%{wks.location}/Vendor/Spdlog/src"
IncludeDir["glfw"] = "%{wks.location}/Vendor/glfw/src"
IncludeDir["glad"] = "%{wks.location}/Vendor/glad/src"
IncludeDir["ImGui"] = "%{wks.location}/Vendor/ImGui/src"
IncludeDir["glm"] = "%{wks.location}/Vendor/glm/src"
IncludeDir["benchmark"] = "%{wks.location}/Benchmarking/Vendor/benchmark/src"
IncludeDir["stb_image"] = "%{wks.location}/Vendor/stb_image/src"
IncludeDir["pugixml"] = "%{wks.location}/Vendor/pugixml/src"

Library = {}
Library["glfw"] = "%{wks.location}/Vendor/glfw/lib/%{cfg.platform}/glfw3_mt.lib"
Library["glad"] = "%{wks.location}/Vendor/glad/lib/%{cfg.platform}/Glad.lib"
Library["benchmark"] = "%{wks.location}/Benchmarking/Vendor/benchmark/lib/%{cfg.platform}-%{cfg.buildcfg}/benchmark.lib"
Library["pugixml"] = "%{wks.location}/Vendor/pugixml/lib/%{cfg.platform}-%{cfg.buildcfg}/pugixml.lib"
