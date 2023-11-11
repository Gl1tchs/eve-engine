set(SOURCES
  asset_library.cc
  asset_library.h
  asset_library.inl
  asset_loader.cc
  asset_loader.h
  asset.h
)

add_library(eve_asset STATIC ${SOURCES})
add_library(eve::asset ALIAS eve_asset)

target_include_directories(eve_asset PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/yaml-cpp/include
)

target_link_libraries(eve_asset PRIVATE
  eve::core
  eve::graphics
  eve::project
  yaml-cpp
)
