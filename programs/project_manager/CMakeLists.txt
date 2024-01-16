set(SOURCE_FILES
  main.cc
  ui_layer.cc
  ui_layer.h
)

add_executable(eve_project_manager ${SOURCE_FILES})

target_include_directories(eve_project_manager PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${ENGINE_DIR}
  ${VENDOR_DIR}/imgui
)

target_link_libraries(eve_project_manager PRIVATE
  eve::core  
  eve::graphics 
  ImGui
)
