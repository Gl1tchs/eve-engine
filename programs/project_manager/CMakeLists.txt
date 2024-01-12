set(SOURCE_FILES
  main.cc
  ui_layer.cc
  ui_layer.h
)

add_executable(eve-project-manager ${SOURCE_FILES})

target_include_directories(eve-project-manager PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${ENGINE_DIR}
  ${VENDOR_DIR}/imgui
)

target_link_libraries(eve-project-manager PRIVATE
  eve::core  
  eve::graphics 
  ImGui
)
