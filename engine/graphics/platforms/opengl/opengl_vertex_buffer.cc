// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_vertex_buffer.h"

#include <glad/glad.h>

namespace eve {
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  glCreateBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  // TODO documentate this constructor and tell the client this defines as DYNAMIC_DRAW
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32_t size) {
  glCreateBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  glDeleteBuffers(1, &vbo_);
}

void OpenGLVertexBuffer::Bind() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void OpenGLVertexBuffer::Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const BufferLayout& OpenGLVertexBuffer::GetLayout() {
  return layout_;
}

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) {
  layout_ = layout;
}
}  // namespace eve