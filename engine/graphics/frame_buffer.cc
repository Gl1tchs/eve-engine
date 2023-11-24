// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/frame_buffer.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_frame_buffer.h"

FrameBufferTextureSpecification::FrameBufferTextureSpecification(
    FrameBufferTextureFormat format)
    : texture_format(format) {}

FrameBufferAttachmentSpecification::FrameBufferAttachmentSpecification(
    std::initializer_list<FrameBufferTextureSpecification> attachments)
    : attachments(attachments) {}

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLFrameBuffer>(spec);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
