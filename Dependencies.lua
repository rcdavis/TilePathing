
IncludeDir = {}
IncludeDir["Spdlog"] = "%{wks.location}/Vendor/Spdlog/src"
IncludeDir["glfw"] = "%{wks.location}/Vendor/glfw/src"
IncludeDir["glad"] = "%{wks.location}/Vendor/glad/src"
IncludeDir["ImGui"] = "%{wks.location}/Vendor/ImGui/src"
IncludeDir["glm"] = "%{wks.location}/Vendor/glm/src"

Library = {}
Library["glfw"] = "%{wks.location}/Vendor/glfw/lib/%{cfg.platform}/glfw3_mt.lib"
Library["glad"] = "%{wks.location}/Vendor/glad/lib/%{cfg.platform}/Glad.lib"
