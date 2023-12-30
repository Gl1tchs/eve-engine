// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/texture.h"

namespace eve {
class OpenGLTexture2D final : public Texture {
 public:
  OpenGLTexture2D(const TextureMetadata& metadata,
                  const void* pixels = nullptr);
  ~OpenGLTexture2D();

  const TextureMetadata& GetMetadata() const override;

  uint32_t GetTextureID() const override;

  void SetData(void* data, uint32_t size) override;

  void Bind(uint16_t slot = 0) const override;

  bool operator==(const Texture& other) const override;

 private:
  TextureMetadata metadata_;

  uint32_t texture_id_;
};
}  // namespace eve