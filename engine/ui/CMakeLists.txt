set(SOURCES
  imgui_layer.cc
  imgui_layer.h
)

set(IMGUI_DIR ${VENDOR_DIR}/imgui)

# TODO if using vulkan change this
list(APPEND SOURCES ${IMGUI_OPENGL_SOURCES})

add_library(eve_ui STATIC ${SOURCES})
add_library(eve::ui ALIAS eve_ui)

target_include_directories(eve_ui PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

target_include_directories(eve_ui PRIVATE
  ${VENDOR_DIR}/imgui
  ${VENDOR_DIR}/glfw/include
)

target_link_libraries(eve_ui PRIVATE
  eve::core
  ImGui
  glfw
)
