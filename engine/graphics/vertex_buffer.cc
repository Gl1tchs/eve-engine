// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/vertex_buffer.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_vertex_buffer.h"

namespace eve {
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLVertexBuffer>(size);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLVertexBuffer>(vertices, size);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}
}  // namespace eve