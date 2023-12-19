set(SOURCES
  imgui_layer.cc
  imgui_layer.h
)

# TODO if using vulkan change this
set(IMGUI_DIR ${VENDOR_DIR}/imgui)
list(APPEND SOURCES 
  ${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.h
)

add_module(ui ${SOURCES})

module_include_directories(ui PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_include_directories(ui PRIVATE
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/imgui
  ${VENDOR_DIR}/include
  ${VENDOR_DIR}/ImGuizmo
)

module_link_libraries(ui PRIVATE
  eve::core
  ImGui
  ImGuizmo
  glfw
)
