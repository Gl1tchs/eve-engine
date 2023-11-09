// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_uniform_buffer.h"

#include <glad/glad.h>

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
  glCreateBuffers(1, &ubo_);
  glNamedBufferData(ubo_, size, nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo_);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() {
  glDeleteBuffers(1, &ubo_);
}

void OpenGLUniformBuffer::SetData(const void* data, uint32_t size,
                                  uint32_t offset) {
  glNamedBufferSubData(ubo_, offset, size, data);
}
