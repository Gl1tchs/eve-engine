set(SOURCES
  engine_api.cc
  engine_api.h
  preprocessor.h
  preprocessor.cc
  script_engine.cc
  script_engine.h
  script.h
  script.cc
)

add_module(scripting ${SOURCES})

module_include_directories(scripting PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/entt/src
  ${VENDOR_DIR}/glm
)

module_include_directories(scripting PRIVATE
  ${VENDOR_DIR}/lua
  ${VENDOR_DIR}/sol2/include
)

module_link_libraries(scripting PRIVATE
  eve::scene
  lua::lua
  sol2::sol2
)

module_precompile_headers(scene PUBLIC ${ENGINE_DIR}/pch_shared.h)
