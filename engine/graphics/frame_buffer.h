// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/texture.h"

class FrameBuffer {
 public:
  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;

  virtual void Refresh() = 0;

  [[nodiscard]] virtual const glm::ivec2& GetSize() = 0;
  virtual void SetSize(glm::ivec2 size) = 0;

  virtual Ref<Texture> GetTexture() = 0;

  [[nodiscard]] static Ref<FrameBuffer> Create(const glm::ivec2 size);
};
