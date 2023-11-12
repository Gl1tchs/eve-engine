cmake_minimum_required(VERSION 3.25)

project(shader_compiler)

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found. Please install the Vulkan SDK.")
endif()

include(cmake/shader-compiler.cmake)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib/${CMAKE_BUILD_TYPE}")

# Set C++ standard and additional flags as needed
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

set(SOURCES
  include/compiler.h
  src/compiler.cc
  src/main.cc
)

add_executable(shader_compiler ${SOURCES})

target_include_directories(shader_compiler PUBLIC
  include/
)

target_include_directories(shader_compiler PRIVATE
  ${VENDOR_DIR}/cxxopts/include
  ${VENDOR_DIR}/SPIRV-Cross
  ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(shader_compiler PRIVATE
  cxxopts
  spirv-cross-core
  spirv-cross-glsl
  Vulkan::shaderc_combined
)
