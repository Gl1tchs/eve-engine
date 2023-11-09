// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/shader.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_shader.h"

Ref<Shader> Shader::Create(const std::string& vs_source,
                           const std::string& fs_source,
                           const std::string& gs_source) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLShader>(vs_source, fs_source, gs_source);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<Shader> Shader::Create(const std::filesystem::path& vs_path,
                           const std::filesystem::path& fs_path,
                           const std::filesystem::path& gs_path) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLShader>(vs_path, fs_path, gs_path);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}