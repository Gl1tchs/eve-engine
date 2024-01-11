set(SOURCES
  asset_loader.cc
  asset_loader.h
  asset_registry.cc
  asset_registry.h
  asset.cc
  asset.h
)

add_module(asset STATIC ${SOURCES})

module_include_directories(asset PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_include_directories(asset PRIVATE
  ${VENDOR_DIR}/json/include
)

module_link_libraries(asset PRIVATE
  eve::core
  eve::graphics
  eve::project
  eve::scene
)

module_precompile_headers(asset PUBLIC ${ENGINE_DIR}/pch_shared.h)
