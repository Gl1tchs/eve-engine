// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/frame_buffer.h"

class OpenGLFrameBuffer final : public FrameBuffer {
 public:
  OpenGLFrameBuffer(const FrameBufferSpecification& spec);
  ~OpenGLFrameBuffer();

  void Bind() const override;

  void Unbind() const override;

  void Invalidate();

  void Resize(int w, int h) override;

  [[nodiscard]] int ReadPixel(uint32_t index, uint32_t x, uint32_t y) override;

  void ClearAttachment(uint32_t attachment_index, int value) override;

  [[nodiscard]] uint32_t GetColorAttachmentRendererID(
      uint32_t index = 0) const override;

  [[nodiscard]] const FrameBufferSpecification& GetSpecification()
      const override {
    return spec_;
  }

 private:
  uint32_t fbo_;

  FrameBufferSpecification spec_;

  std::vector<FrameBufferTextureSpecification> color_attachment_specs_;
  FrameBufferTextureSpecification depth_attachment_spec_;

  std::vector<uint32_t> color_attachments_;
  uint32_t depth_attachment_ = 0;
};
