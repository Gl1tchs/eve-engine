add_subdirectory(entt)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(ImGuizmo)
add_subdirectory(mono)
add_subdirectory(tinyfiledialogs)

set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)

if(ENABLE_TESTING)
  add_subdirectory(Catch2)
endif()
