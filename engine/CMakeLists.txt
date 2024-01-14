include(ModuleHelpers)

add_subdirectory(asset)
add_subdirectory(core)
add_subdirectory(graphics)
add_subdirectory(launch)
add_subdirectory(physics)
add_subdirectory(project)
add_subdirectory(scene)
add_subdirectory(scripting)
add_subdirectory(ui)

add_library(eve INTERFACE)

target_link_libraries(eve INTERFACE
  eve::asset
  eve::core
  eve::graphics
  eve::project
  eve::launch
  eve::physics
  eve::scene
  eve::scripting
  eve::ui
)

add_library(eve::all ALIAS eve)
