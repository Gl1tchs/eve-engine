// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/renderer_api.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_renderer_api.h"

Scope<RendererAPI> RendererAPI::Create() {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateScope<OpenGLRendererAPI>();
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
