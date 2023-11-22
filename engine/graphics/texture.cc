// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "asset/asset_library.h"
#include "core/debug/assert.h"
#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_texture.h"

Ref<Texture> Texture::Create(const TextureMetadata& metadata,
                             const void* pixels) {
  switch (GetGraphicsAPI()) {
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

Ref<Texture> Texture::Create(const std::filesystem::path& path,
                             const TextureMetadata& metadata) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  stbi_uc* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

  ENGINE_ASSERT(data, "Failed to load texture!");

  Ref<Texture> texture = Texture::Create(metadata, data);

  stbi_image_free(data);

  return texture;
}
