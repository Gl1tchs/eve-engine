// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/vertex_buffer.h"

class OpenGLVertexBuffer final : public VertexBuffer {
 public:
  OpenGLVertexBuffer(uint32_t size);
  OpenGLVertexBuffer(const void* vertices, uint32_t size);
  ~OpenGLVertexBuffer();

  void Bind() override;
  void Unbind() override;

  void SetData(const void* data, uint32_t size) override;

  const BufferLayout& GetLayout() override;
  void SetLayout(const BufferLayout& layout) override;

 private:
  uint32_t vbo_;
  BufferLayout layout_;
};
