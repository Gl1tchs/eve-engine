set(SOURCES
  guarded_main.cc
  prelude.h
)

if(WIN32)
  list(APPEND SOURCES
    platform/win32_main.cc
    platform/win32_main.h
  )
elseif(UNIX)
  list(APPEND SOURCES
    platform/linux_main.cc
    platform/linux_main.h
  )
endif()

add_module(launch STATIC ${SOURCES})

module_include_directories(launch PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_link_libraries(launch PRIVATE
  eve::core
  eve::graphics
)

module_precompile_headers(launch PUBLIC ${ENGINE_DIR}/pch_shared.h)
