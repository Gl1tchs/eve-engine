set(SOURCES
  runtime_layer.cc
  runtime_layer.h
  runtime.cc
)

list(APPEND SOURCES ${IMGUI_OPENGL_SOURCES})

add_executable(eve_runtime ${SOURCES})

target_include_directories(eve_runtime PRIVATE
  ${CMAKE_SOURCE_DIR}/runtime
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/entt/src
)

target_link_libraries(eve_runtime PRIVATE
  eve::all
)

target_precompile_headers(eve_runtime PUBLIC pch.h)
