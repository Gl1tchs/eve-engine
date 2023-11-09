// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/texture.h"

#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_texture.h"
#include "texture.h"

Ref<Texture> Texture::Create(const TextureMetadata& metadata,
                             const void* pixels) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLTexture2D>(metadata, pixels);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<Texture> Texture::Create(const std::filesystem::path& path, bool generate_mipmaps) {
  switch (kGraphicsAPI) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLTexture2D>(path, generate_mipmaps);
    case GraphicsAPI::kVulkan:
      ENGINE_ASSERT(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      ENGINE_ASSERT(false, "Unknown graphics API");
      return nullptr;
  }
}
