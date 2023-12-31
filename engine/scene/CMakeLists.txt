set(SOURCES
  components.h
  editor_camera.cc
  editor_camera.h
  entity.cc
  entity.h
  scene_manager.cc
  scene_manager.h
  scene_serializer.cc
  scene_serializer.h
  scene.cc
  scene.h
  static_mesh.cc
  static_mesh.h
  transform.cc
  transform.h
)

add_module(scene STATIC ${SOURCES})

module_include_directories(scene PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_include_directories(scene PRIVATE
  ${VENDOR_DIR}/entt/src
  ${VENDOR_DIR}/json/include
  ${VENDOR_DIR}/include
  ${VENDOR_DIR}/OBJ-Loader
)

module_link_libraries(scene PRIVATE
  eve::asset
  eve::core
  eve::scripting
  EnTT
)

module_precompile_headers(scene PUBLIC ${ENGINE_DIR}/pch_shared.h)
