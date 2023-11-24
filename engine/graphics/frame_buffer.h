// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <initializer_list>
#include <vector>

#include <glm/glm.hpp>

#include "core/utils/memory.h"

enum class FrameBufferTextureFormat {
  kNone = 0,
  // Color
  kRGB8,
  kRGBA8,
  kRGBA16F,
  kRGBA32F,
  kRG32F,
  kR32I,
  // Depth
  kDepth32FStencil8,
  kDepth24Stencil8,
  // Defaults
  kDepth = kDepth24Stencil8
};

struct FrameBufferTextureSpecification {
  FrameBufferTextureSpecification() = default;
  FrameBufferTextureSpecification(FrameBufferTextureFormat format);

  FrameBufferTextureFormat texture_format = FrameBufferTextureFormat::kNone;
  // TODO: filtering/wrap
};

struct FrameBufferAttachmentSpecification {
  FrameBufferAttachmentSpecification() = default;
  FrameBufferAttachmentSpecification(
      std::initializer_list<FrameBufferTextureSpecification> attachments);

  std::vector<FrameBufferTextureSpecification> attachments;
};

struct FrameBufferSpecification {
  uint32_t width = 0, height = 0;
  FrameBufferAttachmentSpecification attachments;
  uint32_t samples = 1;

  bool swap_chain_targets = false;
};

class FrameBuffer {
 public:
  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;

  virtual void Resize(int w, int h) = 0;

  [[nodiscard]] virtual int ReadPixel(uint32_t index, uint32_t x,
                                      uint32_t y) = 0;

  virtual void ClearAttachment(uint32_t attachment_index, int value) = 0;

  [[nodiscard]] virtual uint32_t GetColorAttachmentRendererID(
      uint32_t index = 0) const = 0;

  [[nodiscard]] virtual const FrameBufferSpecification& GetSpecification()
      const = 0;

  [[nodiscard]] static Ref<FrameBuffer> Create(
      const FrameBufferSpecification& spec);
};
