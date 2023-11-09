set(SOURCES
  debug/instrumentor.cc
  debug/instrumentor.h
  debug/instrumentor.inc
  debug/log.cc
  debug/log.h
  event/event_handler.h
  event/event_handler.inc
  event/events.h
  event/key_code.h
  event/key_event.h
  event/mouse_code.h
  event/mouse_event.h
  event/window_event.h
  utils/memory.h
  utils/memory.inc
  utils/timer.cc
  utils/timer.h
  core_minimal.h
  window.cc
  window.h
)

add_library(eve_core STATIC ${SOURCES})
add_library(eve::core ALIAS eve_core)

target_include_directories(eve_core PUBLIC
  ${CMAKE_SOURCE_DIR}/engine/
  ${VENDOR_DIR}/glm/
)

target_include_directories(eve_core PRIVATE
  ${glfw_IncludeDirs}
  ${spdlog_IncludeDirectories}
)

target_link_libraries(eve_core PRIVATE glfw glm spdlog)

expose_headers(eve_core ${CMAKE_CURRENT_LIST_DIR})
