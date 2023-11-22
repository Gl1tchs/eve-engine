set(SOURCES
  components.h
  editor_camera.cc
  editor_camera.h
  entity.cc
  entity.h
  lights.h
  scene_serializer.cc
  scene_serializer.h
  scene.cc
  scene.h
  static_mesh.cc
  static_mesh.h
  transform.cc
  transform.h
)

add_library(eve_scene STATIC ${SOURCES})
add_library(eve::scene ALIAS eve_scene)

target_include_directories(eve_scene PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

target_include_directories(eve_scene PRIVATE
  ${VENDOR_DIR}/entt/src
  ${VENDOR_DIR}/yaml-cpp/include
  ${VENDOR_DIR}/include
)

target_link_libraries(eve_scene PRIVATE
  eve::asset
  eve::core
  EnTT
  yaml-cpp
)
