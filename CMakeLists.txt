cmake_minimum_required(VERSION 3.25)

project(eve)

include(CMakeSettings.cmake)

set(EDITOR_DIR ${CMAKE_SOURCE_DIR}/editor)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# Define the targets
add_subdirectory(editor)
add_subdirectory(engine)
add_subdirectory(thirdparty)
