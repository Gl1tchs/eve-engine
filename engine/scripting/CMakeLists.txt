set(SOURCES
  script_engine.cc
  script_engine.h
  script_glue.cc
  script_glue.h
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
  ${VENDOR_DIR}/mono/include/mono-2.0
  ${VENDOR_DIR}/filewatcher
)

module_link_libraries(scripting PRIVATE Mono)

module_precompile_headers(scene PUBLIC ${ENGINE_DIR}/pch_shared.h)
