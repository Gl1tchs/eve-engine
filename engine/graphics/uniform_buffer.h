// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {
class UniformBuffer {
 public:
  virtual void SetData(const void* data, uint32_t size,
                       uint32_t offset = 0) = 0;

  [[nodiscard]] static Ref<UniformBuffer> Create(uint32_t size,
                                                 uint32_t binding);
};
}  // namespace eve