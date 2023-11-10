set(SOURCES
  components.h
  editor_camera.cc
  editor_camera.h
  entity.cc
  entity.h
  scene.cc
  scene.h
)

add_library(eve_scene STATIC ${SOURCES})
add_library(eve::scene ALIAS eve_scene)

target_include_directories(eve_scene PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

target_include_directories(eve_scene PRIVATE
  ${VENDOR_DIR}/entt/src
)

target_link_libraries(eve_scene PRIVATE
  EnTT
  eve::core
)
