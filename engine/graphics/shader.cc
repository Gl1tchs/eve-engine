// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/shader.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_shader.h"

namespace eve {

Ref<Shader> Shader::Create(const std::string& vs_path,
                           const std::string& fs_path,
                           const std::string& custom_shader) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLShader>(vs_path, fs_path, custom_shader);
    case GraphicsAPI::kVulkan:
      ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}

}  // namespace eve