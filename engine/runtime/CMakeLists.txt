set(SOURCES
  launch/guarded_main.cc
  prelude.h
)

if(${EVE_PLATFORM} STREQUAL WIN32)
  list(APPEND SOURCES
    launch/win32_main.cc
    launch/win32_main.h
  )
elseif(${EVE_PLATFORM} STREQUAL LINUX)
  list(APPEND SOURCES
    launch/linux_main.cc
    launch/linux_main.h
  )
endif()

add_module(runtime STATIC ${SOURCES})

module_include_directories(runtime PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_link_libraries(runtime PRIVATE
  eve::core
  eve::graphics
)

module_precompile_headers(runtime PUBLIC ${ENGINE_DIR}/pch_shared.h)
