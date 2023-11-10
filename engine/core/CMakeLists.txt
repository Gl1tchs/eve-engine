set(SOURCES
  debug/instrumentor.cc
  debug/instrumentor.h
  debug/instrumentor.inl
  debug/log.cc
  debug/log.h
  event/event_handler.h
  event/event_handler.inl
  event/events.h
  event/input.cc
  event/input.h
  event/key_code.h
  event/key_event.h
  event/mouse_code.h
  event/mouse_event.h
  event/window_event.h
  math/transform.cc
  math/transform.h
  utils/guuid.cc
  utils/guuid.h
  utils/memory.h
  utils/memory.inl
  utils/timer.cc
  utils/timer.h
  core_minimal.h
  instance.cc
  instance.h
  layer_stack.cc
  layer_stack.h
  layer.cc
  layer.h
  state.h
  window.cc
  window.h
)

add_library(eve_core STATIC ${SOURCES})
add_library(eve::core ALIAS eve_core)

target_include_directories(eve_core PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/glm
)

target_include_directories(eve_core PRIVATE
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/spdlog/include
)

target_link_libraries(eve_core PRIVATE
  glfw
  glm
  spdlog
  eve::ui
)

target_compile_definitions(eve_core PRIVATE GLFW_INCLUDE_NONE)
