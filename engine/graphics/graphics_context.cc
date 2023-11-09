// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/graphics_context.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_context.h"

Ref<GraphicsContext> GraphicsContext::Create() {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLContext>();
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
