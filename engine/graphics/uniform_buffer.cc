// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/uniform_buffer.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_uniform_buffer.h"

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLUniformBuffer>(size, binding);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
