set(SOURCES
  layers/editor_layer.cc
  layers/editor_layer.h
  modals/exit_modal.cc
  modals/exit_modal.h
  modals/modal.cc
  modals/modal.h
  panels/about_panel.cc
  panels/about_panel.h
  panels/asset_registry_panel.cc
  panels/asset_registry_panel.h
  panels/console_panel.cc
  panels/console_panel.h
  panels/content_browser_panel.cc
  panels/content_browser_panel.h
  panels/debug_info_panel.cc
  panels/debug_info_panel.h
  panels/hierarchy_panel.cc
  panels/hierarchy_panel.h
  panels/inspector_panel.cc
  panels/inspector_panel.h
  panels/panel.cc
  panels/panel.h
  panels/project_settings_panel.cc
  panels/project_settings_panel.h
  panels/scene_settings_panel.cc
  panels/scene_settings_panel.h
  panels/toolbar_panel.cc
  panels/toolbar_panel.h
  panels/viewport_panel.cc
  panels/viewport_panel.h
  utils/cpuid.cc
  utils/cpuid.h
  utils/imgui_utils.cc
  utils/imgui_utils.h
  utils/modify_info.h
  widgets/dock_space.cc
  widgets/dock_space.h
  widgets/menu_bar.cc
  widgets/menu_bar.h
  editor.cc
)

list(APPEND SOURCES ${IMGUI_OPENGL_SOURCES})

add_executable(eve_editor ${SOURCES})

target_include_directories(eve_editor PRIVATE
  ${CMAKE_SOURCE_DIR}/editor
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/imgui
  ${VENDOR_DIR}/ImGuizmo
  ${VENDOR_DIR}/include
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/entt/src
  ${VENDOR_DIR}/tinyfiledialogs
)

target_link_libraries(eve_editor PRIVATE
  eve::all
  glfw # for imgui
  ImGui
  ImGuizmo
  tinyfiledialogs
)

target_precompile_headers(eve_editor PUBLIC pch.h)

add_custom_command(TARGET eve_editor PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/LICENSE
  COMMENT "Copying LICENSE to the build destination."
)

add_custom_command(TARGET eve_editor PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${VENDOR_DIR}/mono/bin/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
  COMMENT "Copying LICENSE to the build destination."
)
