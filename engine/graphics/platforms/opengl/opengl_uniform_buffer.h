// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/uniform_buffer.h"

class OpenGLUniformBuffer final : public UniformBuffer {
 public:
  OpenGLUniformBuffer(uint32_t size, uint32_t binding);
  ~OpenGLUniformBuffer();

  void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

 private:
  uint32_t ubo_;
};
