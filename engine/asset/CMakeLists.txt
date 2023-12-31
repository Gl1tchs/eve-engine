set(SOURCES
  asset_library.cc
  asset_library.h
  asset_library.inl
  asset_loader.cc
  asset_loader.h
  asset.h
)

add_module(asset STATIC ${SOURCES})

module_include_directories(asset PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_link_libraries(asset PRIVATE
  eve::core
  eve::graphics
  eve::project
  eve::scene
)

module_precompile_headers(asset PUBLIC ${ENGINE_DIR}/pch_shared.h)
