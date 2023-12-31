// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/index_buffer.h"
#include "graphics/vertex_buffer.h"

namespace eve {
class VertexArray {
 public:
  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers()
      const = 0;
  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;

  [[nodiscard]] virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
  virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

  [[nodiscard]] static Ref<VertexArray> Create();
};
}  // namespace eve