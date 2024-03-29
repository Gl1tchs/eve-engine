// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/vertex_array.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_vertex_array.h"

namespace eve {
  
Ref<VertexArray> VertexArray::Create() {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLVertexArray>();
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

}  // namespace eve