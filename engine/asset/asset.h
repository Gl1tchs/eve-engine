// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"

#define EVE_IMPL_ASSET(type)                     \
  constexpr AssetType GetType() const override { \
    return type;                                 \
  };

namespace eve {

typedef UUID AssetHandle;

enum class AssetType : uint8_t {
  kNone = 0,
  kTexture,
  kFont,
  kScene,
  kStaticMesh,
  kScript,
  kMaterial,
  kShader,
  kAudio,
};

AssetType GetAssetTypeFromExtension(const std::string& extension);

std::string DeserializeAssetType(AssetType type);

struct Asset {
  AssetHandle handle = 0;
  std::string name = "";
  std::string path = "";

  virtual constexpr AssetType GetType() const = 0;
};

}  // namespace eve