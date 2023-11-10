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

add_library(eve_runtime STATIC ${SOURCES})
add_library(eve::runtime ALIAS eve_runtime)

target_include_directories(eve_runtime PRIVATE
  ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(eve_runtime PRIVATE
  eve::core
  eve::graphics
)
