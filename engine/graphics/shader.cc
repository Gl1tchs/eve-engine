// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/shader.h"

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_shader.h"

Ref<Shader> Shader::Create(const std::string& vs_path,
                           const std::string& fs_path,
                           const std::string& gs_path) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLShader>(vs_path, fs_path, gs_path);
    case GraphicsAPI::kVulkan:
      ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}