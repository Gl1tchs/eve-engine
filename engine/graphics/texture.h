// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"

namespace eve {

enum class TextureType { kDiffuse, kSpecular, kNormal, kHeight };

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
  glm::ivec2 size = {1, 1};
  TextureType type = TextureType::kDiffuse;
  TextureFormat format = TextureFormat::kRGBA;
  TextureFilteringMode min_filter = TextureFilteringMode::kLinear;
  TextureFilteringMode mag_filter = TextureFilteringMode::kLinear;
  TextureWrappingMode wrap_s = TextureWrappingMode::kRepeat;
  TextureWrappingMode wrap_t = TextureWrappingMode::kRepeat;
  bool generate_mipmaps = true;
};

class Texture : public Asset {
 public:
  IMPL_ASSET(AssetType::kTexture)

  virtual const TextureMetadata& GetMetadata() const = 0;

  virtual uint32_t GetTextureID() const = 0;

  virtual void SetData(void* data, uint32_t size) = 0;

  virtual void Bind(uint16_t slot = 0) const = 0;

  virtual bool operator==(const Texture& other) const = 0;

  [[nodiscard]] static Ref<Texture> Create(const TextureMetadata& metadata,
                                           const void* pixels);

  [[nodiscard]] static Ref<Texture> Create(
      const fs::path& path, const TextureType& type = TextureType::kDiffuse);
};

}  // namespace eve