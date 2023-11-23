set(SOURCES
  layers/editor_layer.cc
  layers/editor_layer.h
  modals/exit_modal.cc
  modals/exit_modal.h
  modals/importer_modal.cc
  modals/importer_modal.h
  modals/modal.cc
  modals/modal.h
  panels/hierarchy_panel.cc
  panels/hierarchy_panel.h
  panels/inspector_panel.cc
  panels/inspector_panel.h
  panels/panel.cc
  panels/panel.h
  panels/render_stats_panel.cc
  panels/render_stats_panel.h
  panels/toolbar_panel.cc
  panels/toolbar_panel.h
  panels/viewport_panel.cc
  panels/viewport_panel.h
  utils/imgui_utils.cc
  utils/imgui_utils.h
  utils/modify_info.h
  widgets/combo.cc
  widgets/combo.h
  widgets/dock_space.cc
  widgets/dock_space.h
  widgets/menu_bar.cc
  widgets/menu_bar.h
  editor.cc
)

list(APPEND SOURCES ${IMGUI_OPENGL_SOURCES})

if(${EVE_PLATFORM} STREQUAL WIN32 AND CMAKE_BUILD_TYPE STREQUAL "dist")
  add_executable(eve_editor WIN32 ${SOURCES})
elseif(${EVE_PLATFORM} STREQUAL LINUX OR ${EVE_PLATFORM} STREQUAL WIN32)
  add_executable(eve_editor ${SOURCES})
else()
  message(FATAL_ERROR "Editor cannot run except windows or linux!")
endif()

target_include_directories(eve_editor PRIVATE
  ${CMAKE_SOURCE_DIR}/editor
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/cxxopts/include
  ${VENDOR_DIR}/imgui
  ${VENDOR_DIR}/ImGuizmo
  ${VENDOR_DIR}/include
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/entt/src
  ${VENDOR_DIR}/tinyfiledialogs
)

target_link_libraries(eve_editor PRIVATE
  eve::all
  cxxopts
  glfw # for imgui
  ImGui
  ImGuizmo
  EnTT
  tinyfiledialogs
)

add_custom_command(TARGET eve_editor PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  COMMENT "Copying assets to the build destination."
)
