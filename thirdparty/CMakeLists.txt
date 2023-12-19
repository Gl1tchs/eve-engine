add_subdirectory(cxxopts)
add_subdirectory(entt)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(ImGuizmo)

set(LUA_SRC_DIR ${VENDOR_DIR}/lua)
add_subdirectory(luacpp)

option(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(yaml-cpp)

add_subdirectory(tinyfiledialogs)