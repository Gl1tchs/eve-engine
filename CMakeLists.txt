cmake_minimum_required(VERSION 3.25)

project(eve_engine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(header-expose)

# Options
option(ENABLE_TESTING "Should enable testing?" ON)
set(ENABLE_TESTING ${ENABLE_TESTING} CACHE BOOL "Should enable testing?" FORCE)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib/${CMAKE_BUILD_TYPE}")

# Define the configurations
set(CMAKE_CONFIGURATION_TYPES "debug;release;dist")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# Set C++ standard and additional flags as needed
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set specific flags for each configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEVE_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DEVE_RELEASE -O2")
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_DIST} -DEVE_DIST -O2")

# Define the targets
add_subdirectory(editor)
add_subdirectory(engine)
add_subdirectory(thirdparty)
