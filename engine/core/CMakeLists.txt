set(SOURCES
  debug/log.cc
  debug/log.h
  event/event_handler.h
  event/event_handler.inl
  event/input.cc
  event/input.h
  event/key_code.h
  event/key_event.h
  event/mouse_code.h
  event/mouse_event.h
  event/window_event.h
  math/box.h
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

add_module(core ${SOURCES})

module_include_directories(core PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/glm
)

module_include_directories(core PRIVATE
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/spdlog/include
)

module_link_libraries(core PRIVATE
  eve::asset
  eve::ui
  glfw
  glm
)

module_precompile_headers(core PUBLIC ${ENGINE_DIR}/pch_shared.h)

module_compile_definitions(core PRIVATE GLFW_INCLUDE_NONE)
