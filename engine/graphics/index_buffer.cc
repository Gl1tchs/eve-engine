// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/index_buffer.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_index_buffer.h"

namespace eve {

Ref<IndexBuffer> IndexBuffer::Create(uint32_t size) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLIndexBuffer>(size);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLIndexBuffer>(indices, count);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

}  // namespace eve