set(SOURCES
  editor.cc
)

if(${EVE_PLATFORM} STREQUAL WIN32)
  add_executable(eve_editor WIN32 ${SOURCES})
elseif(${EVE_PLATFORM} STREQUAL LINUX)
  add_executable(eve_editor ${SOURCES})
else()
  message(FATAL_ERROR "Editor cannot run except windows or linux!")
endif()

target_include_directories(eve_editor PRIVATE
  ${CMAKE_SOURCE_DIR}/editor/
  ${CMAKE_SOURCE_DIR}/engine/
)

target_link_libraries(eve_editor PRIVATE eve::all)
