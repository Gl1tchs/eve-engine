set(SOURCES
  asset_library.cc
  asset_library.h
  asset_library.inl
  asset_loader.cc
  asset_loader.h
  asset.cc
  asset.h
)

add_module(asset STATIC ${SOURCES})

module_include_directories(asset PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/yaml-cpp/include
)

module_link_libraries(asset PRIVATE
  eve::core
  eve::graphics
  eve::project
  eve::scene
  yaml-cpp
)

module_precompile_headers(asset PUBLIC ${ENGINE_DIR}/pch_shared.h)
