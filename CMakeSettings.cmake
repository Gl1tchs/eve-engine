set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake/utility")

include(module_helpers)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib/${CMAKE_BUILD_TYPE}")

# Define the configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist")
if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "Configuration not found using default (Debug) instead!")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# Set C++ standard and additional flags as needed
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set specific flags for each configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_DIST} -O2")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(EVE_DEBUG)
    add_compile_definitions(EVE_ENABLE_PROFILING)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(EVE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Dist")
    add_compile_definitions(EVE_DIST)
endif()

# Set definitions
if(WIN32)
    set(EVE_PLATFORM "WIN32")    
    add_compile_definitions(EVE_PLATFORM_WIN32)
elseif(UNIX AND NOT APPLE)
    set(EVE_PLATFORM "LINUX")    
    add_compile_definitions(EVE_PLATFORM_LINUX)
else()
    message(LOG_FATAL "Unsupported platform!")
endif()