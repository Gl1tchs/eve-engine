// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/graphics_context.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_context.h"

Ref<GraphicsContext> GraphicsContext::Create() {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLContext>();
    case GraphicsAPI::kVulkan:
      ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
