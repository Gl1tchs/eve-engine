add_subdirectory("core")
add_subdirectory("runtime")

add_library(eve INTERFACE)

target_link_libraries(eve INTERFACE eve_core eve_runtime)

add_library(eve::all ALIAS eve)
