cmake_minimum_required(VERSION 3.25)

project(eve VERSION 0.1.0 LANGUAGES CXX CSharp)

include(CMakeSettings.cmake)
include(ModuleHelpers)

set(EDITOR_DIR ${CMAKE_SOURCE_DIR}/editor)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# Define the targets
add_subdirectory(editor)
add_subdirectory(engine)
add_subdirectory(programs)
add_subdirectory(runtime)
add_subdirectory(script-core)
add_subdirectory(thirdparty)
