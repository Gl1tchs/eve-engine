set(SOURCES
  project_serializer.cc
  project_serializer.h
  project.cc
  project.h
)

add_module(project ${SOURCES})

module_include_directories(project PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

module_include_directories(project PRIVATE
  ${VENDOR_DIR}/yaml-cpp/include
)

module_precompile_headers(graphics PUBLIC ${ENGINE_DIR}/pch_shared.h)

module_link_libraries(project PRIVATE
  eve::core
  yaml-cpp
)
