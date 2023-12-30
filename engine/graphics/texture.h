// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {
enum class TextureFormat {
  kRed,
  kRG,
  kRGB,
  kBGR,
  kRGBA,
  kBGRA,
};

enum class TextureFilteringMode {
  kNearest,
  kLinear,
};

enum class TextureWrappingMode {
  kRepeat,
  kMirroredRepeat,
  kClampToEdge,
  kClampToBorder,
};

struct TextureMetadata final {
  // will be setted by texture importer
  glm::ivec2 size;
  TextureFormat format;
  TextureFilteringMode min_filter;
  TextureFilteringMode mag_filter;
  TextureWrappingMode wrap_s;
  TextureWrappingMode wrap_t;
  bool generate_mipmaps = true;
};

class Texture {
 public:
  virtual const TextureMetadata& GetMetadata() const = 0;

  virtual uint32_t GetTextureID() const = 0;

  virtual void SetData(void* data, uint32_t size) = 0;

  virtual void Bind(uint16_t slot = 0) const = 0;

  virtual bool operator==(const Texture& other) const = 0;

  [[nodiscard]] static Ref<Texture> Create(const TextureMetadata& metadata,
                                           const void* pixels);

  [[nodiscard]] static Ref<Texture> Create(const std::filesystem::path& path,
                                           TextureMetadata metadata);
};
}  // namespace eve