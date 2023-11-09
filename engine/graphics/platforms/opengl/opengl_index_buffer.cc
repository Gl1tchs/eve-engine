// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_index_buffer.h"

#include <glad/glad.h>

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size) {
  glCreateBuffers(1, &ibo_);
  glBindBuffer(GL_ARRAY_BUFFER, ibo_);
  // TODO documentate this constructor and tell the client this defines as DYNAMIC_DRAW
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
  glCreateBuffers(1, &ibo_);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
  glBindBuffer(GL_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  glDeleteBuffers(1, &ibo_);
}

void OpenGLIndexBuffer::Bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
}

void OpenGLIndexBuffer::Unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLIndexBuffer::SetData(const void* data, uint32_t size) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

uint32_t OpenGLIndexBuffer::GetCount() {
  return count_;
}
