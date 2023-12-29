cmake_minimum_required(VERSION 3.25)
project(Mono)

add_library(Mono INTERFACE)

target_include_directories(Mono INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(Mono INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/mono-2.0-sgen.lib)
