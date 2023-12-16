add_subdirectory("cxxopts")
add_subdirectory("entt")
add_subdirectory("glad")
add_subdirectory("glfw")
add_subdirectory("glm")
add_subdirectory("imgui")
add_subdirectory("ImGuizmo")

option(YAML_CPP_BUILD_CONTRIB OFF)
option(YAML_CPP_BUILD_TOOLS OFF)
option(YAML_BUILD_SHARED_LIBS OFF)
add_subdirectory("yaml-cpp")

add_subdirectory("tinyfiledialogs")