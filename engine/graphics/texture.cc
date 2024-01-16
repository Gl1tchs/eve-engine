// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "graphics/graphics.h"
#include "graphics/platforms/opengl/opengl_texture.h"

namespace eve {
Ref<Texture> Texture::Create(const TextureMetadata& metadata,
                             const void* pixels) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLTexture2D>(metadata, pixels);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<Texture> Texture::Create(const fs::path& path, const TextureType& type) {
  int width, height, channels;
  stbi_uc* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

  if (!data) {
    stbi_image_free(data);
    EVE_LOG_ENGINE_ERROR("Unable to load texture from: {}", path.string());
    EVE_ASSERT_ENGINE(false);
  }

  TextureMetadata metadata;
  metadata.size = glm::ivec2{width, height};
  metadata.type = type;
  metadata.min_filter = TextureFilteringMode::kLinear;
  metadata.mag_filter = TextureFilteringMode::kLinear;
  metadata.wrap_s = TextureWrappingMode::kRepeat;
  metadata.wrap_t = TextureWrappingMode::kRepeat;
  metadata.generate_mipmaps = true;

  switch (channels) {
    case 1:
      metadata.format = TextureFormat::kRed;
      break;
    case 2:
      metadata.format = TextureFormat::kRG;
      break;
    case 3:
      metadata.format = TextureFormat::kRGB;
      break;
    case 4:
      metadata.format = TextureFormat::kRGBA;
      break;
    default:
      EVE_ASSERT_ENGINE(false, "Unsupported number of channels in the image");
      break;
  }

  Ref<Texture> texture = Texture::Create(metadata, data);

  stbi_image_free(data);

  return texture;
}
}  // namespace eve