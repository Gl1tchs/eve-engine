// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/frame_buffer.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_frame_buffer.h"

namespace eve {
Ref<FrameBuffer> FrameBuffer::Create(const glm::ivec2 size) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLFrameBuffer>(size);
    case GraphicsAPI::kVulkan:
      ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
}  // namespace eve