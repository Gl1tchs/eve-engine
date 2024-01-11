set(SOURCES
  box_collider.cc
  box_collider.h
  collider.h
  physics_world.cc
  physics_world.h
  rigidbody.cc
  rigidbody.h
)

add_module(physics ${SOURCES})

module_include_directories(physics PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/entt/src
)

module_precompile_headers(graphics PUBLIC ${ENGINE_DIR}/pch_shared.h)

module_link_libraries(physics PRIVATE
  eve::core
  eve::scene
)
