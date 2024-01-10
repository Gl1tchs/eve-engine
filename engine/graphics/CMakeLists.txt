set(SOURCES
  primitives/line.cc
  primitives/line.h
  primitives/mesh.cc
  primitives/mesh.h
  primitives/primitive.h
  primitives/quad.cc
  primitives/quad.h
  buffer_layout.cc
  buffer_layout.h
  camera.h
  frame_buffer.cc
  frame_buffer.h
  graphics_context.cc
  graphics_context.h
  graphics.h
  index_buffer.cc
  index_buffer.h
  material.cc
  material.h
  orthographic_camera.cc
  orthographic_camera.h
  perspective_camera.cc
  perspective_camera.h
  render_command.cc
  render_command.h
  renderer_api.cc
  renderer_api.h
  renderer.cc
  renderer.h
  scene_renderer.cc
  scene_renderer.h
  shader.cc
  shader.h
  texture.cc
  texture.h
  uniform_buffer.cc
  uniform_buffer.h
  vertex_array.cc
  vertex_array.h
  vertex_buffer.cc
  vertex_buffer.h
)

set(OPENGL_SOURCES
  platforms/opengl/opengl_context.cc
  platforms/opengl/opengl_context.h
  platforms/opengl/opengl_frame_buffer.cc
  platforms/opengl/opengl_frame_buffer.h
  platforms/opengl/opengl_index_buffer.cc
  platforms/opengl/opengl_index_buffer.h
  platforms/opengl/opengl_renderer_api.cc
  platforms/opengl/opengl_renderer_api.h
  platforms/opengl/opengl_shader.cc
  platforms/opengl/opengl_shader.h
  platforms/opengl/opengl_texture.cc
  platforms/opengl/opengl_texture.h
  platforms/opengl/opengl_uniform_buffer.cc
  platforms/opengl/opengl_uniform_buffer.h
  platforms/opengl/opengl_vertex_array.cc
  platforms/opengl/opengl_vertex_array.h
  platforms/opengl/opengl_vertex_buffer.cc
  platforms/opengl/opengl_vertex_buffer.h
)

# TODO add this as an option
list(APPEND SOURCES ${OPENGL_SOURCES})

add_module(graphics ${SOURCES})

module_include_directories(graphics PUBLIC
  ${CMAKE_SOURCE_DIR}/engine
  ${VENDOR_DIR}/glm
)
  
module_include_directories(graphics PRIVATE
  ${VENDOR_DIR}/entt/src
  ${VENDOR_DIR}/glad/include/
  ${VENDOR_DIR}/glfw/include
  ${VENDOR_DIR}/stb
)

module_link_libraries(graphics PRIVATE
  eve::asset 
  eve::core
  eve::scene
  glm
  glad
  glfw
)

module_precompile_headers(graphics PUBLIC ${ENGINE_DIR}/pch_shared.h)

module_compile_definitions(graphics PRIVATE GLFW_INCLUDE_NONE)
