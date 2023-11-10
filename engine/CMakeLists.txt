add_subdirectory("core")
add_subdirectory("graphics")
add_subdirectory("runtime")

add_library(eve INTERFACE)

target_link_libraries(eve INTERFACE
  eve::core
  eve::graphics
  eve::runtime
)

add_library(eve::all ALIAS eve)
