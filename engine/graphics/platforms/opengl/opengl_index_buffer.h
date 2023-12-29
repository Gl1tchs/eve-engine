// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/index_buffer.h"

class OpenGLIndexBuffer final : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t size);
  OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
  ~OpenGLIndexBuffer();

  void Bind() override;
  void Unbind() override;

  void SetData(const void* data, uint32_t size) override;

  uint32_t GetCount() override;

 private:
  uint32_t ibo_;
  uint32_t count_;
};
