// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/frame_buffer.h"

class OpenGLFrameBuffer final : public FrameBuffer {
 public:
  OpenGLFrameBuffer(const glm::ivec2& size);
  ~OpenGLFrameBuffer();

  void Bind() const override;

  void Unbind() const override;

  void Refresh() override;

  [[nodiscard]] const glm::ivec2& GetSize() override;
  void SetSize(glm::ivec2 size) override;

  Ref<Texture> GetTexture() override;

 private:
  glm::ivec2 size_;

  uint32_t fbo_;
  uint32_t rbo_;

  Ref<Texture> texture_;
};
