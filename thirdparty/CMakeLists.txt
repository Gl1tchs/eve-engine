add_subdirectory(cxxopts)
add_subdirectory(entt)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(ImGuizmo)

option(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(yaml-cpp)

add_subdirectory(tinyfiledialogs)

add_subdirectory(lua)

set(SOL2_LUA_VERSION "5.4.6" CACHE STRING "The version of Lua used")
set(SOL2_BUILD_LUA FALSE CACHE BOOL "Always build Lua, do not search for it in the system")
add_subdirectory(sol2)