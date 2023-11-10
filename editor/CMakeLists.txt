set(SOURCES
  layers/editor_layer.cc
  layers/editor_layer.h
  editor.cc
)

list(APPEND SOURCES ${IMGUI_OPENGL_SOURCES})

if(${EVE_PLATFORM} STREQUAL WIN32)
  add_executable(eve_editor WIN32 ${SOURCES})
elseif(${EVE_PLATFORM} STREQUAL LINUX)
  add_executable(eve_editor ${SOURCES})
else()
  message(FATAL_ERROR "Editor cannot run except windows or linux!")
endif()

target_include_directories(eve_editor PRIVATE
  ${CMAKE_SOURCE_DIR}/editor
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/imgui
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/entt/src
)

target_link_libraries(eve_editor PRIVATE
  ImGui
  glfw # for imgui
  EnTT
  eve::all
)

add_custom_command(TARGET eve_editor PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  COMMENT "Copying assets to the build destination."
)
