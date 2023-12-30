// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/buffer_layout.h"

namespace eve {
class VertexBuffer {
 public:
  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void SetData(const void* data, uint32_t size) = 0;

  virtual const BufferLayout& GetLayout() = 0;
  virtual void SetLayout(const BufferLayout& layout) = 0;

  [[nodiscard]] static Ref<VertexBuffer> Create(uint32_t size);

  [[nodiscard]] static Ref<VertexBuffer> Create(const void* vertices,
                                                uint32_t size);
};
}  // namespace eve