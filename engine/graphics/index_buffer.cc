// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/index_buffer.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_index_buffer.h"

Ref<IndexBuffer> IndexBuffer::Create(uint32_t size) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLIndexBuffer>(size);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLIndexBuffer>(indices, count);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
