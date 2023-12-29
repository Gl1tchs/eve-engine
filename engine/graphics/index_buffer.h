// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

class IndexBuffer {
 public:
  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void SetData(const void* data, uint32_t size) = 0;

  virtual uint32_t GetCount() = 0;

  [[nodiscard]] static Ref<IndexBuffer> Create(uint32_t size);

  [[nodiscard]] static Ref<IndexBuffer> Create(const uint32_t* indices,
                                               uint32_t count);
};
