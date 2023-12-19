set(SOURCES
  script_engine.cc
  script_engine.h
  script.h
  script.cc
)

add_module(scripting ${SOURCES})

module_include_directories(scripting PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/glm
)

module_include_directories(scripting PRIVATE
  ${VENDOR_DIR}/luacpp/include
)

module_link_libraries(scripting PRIVATE
  luacpp_static
)

module_precompile_headers(scene PUBLIC ${ENGINE_DIR}/pch_shared.h)
