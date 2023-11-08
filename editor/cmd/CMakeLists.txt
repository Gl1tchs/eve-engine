set(SOURCES
  main.cc
)

add_executable(eve_editor ${SOURCES})

target_include_directories(eve_editor PRIVATE
  ${CMAKE_SOURCE_DIR}/editor/
  ${CMAKE_SOURCE_DIR}/engine/
)

target_link_libraries(eve_editor PRIVATE eve_core)
