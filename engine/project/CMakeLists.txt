set(SOURCES
  project_serializer.cc
  project_serializer.h
  project.cc
  project.h
)

add_library(eve_project STATIC ${SOURCES})
add_library(eve::project ALIAS eve_project)

target_include_directories(eve_project PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
)

target_include_directories(eve_project PRIVATE
  ${VENDOR_DIR}/yaml-cpp/include
)

target_link_libraries(eve_project PRIVATE
  eve::core
  yaml-cpp
)
