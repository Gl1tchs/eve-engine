// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/vertex_array.h"

class OpenGLVertexArray final : public VertexArray {
 public:
  OpenGLVertexArray();
  ~OpenGLVertexArray();

  void Bind() const;
  void Unbind() const;

  [[nodiscard]] const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const;
  void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer);

  [[nodiscard]] const Ref<IndexBuffer>& GetIndexBuffer() const;
  void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer);

 private:
  uint32_t vao_;
  uint32_t vertex_buffer_index_ = 0;
  std::vector<Ref<VertexBuffer>> vertex_buffers_;
  Ref<IndexBuffer> index_buffer_;
};
