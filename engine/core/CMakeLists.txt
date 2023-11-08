set(SOURCES
  foo.h
  foo.cc
)

add_library(eve_core STATIC ${SOURCES})

target_include_directories(eve_core PRIVATE
  ${CMAKE_SOURCE_DIR}/engine/
)

expose_headers(eve_core ${CMAKE_CURRENT_LIST_DIR})
