// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"

namespace eve {

enum class AssetType { kTexture, kFont, kScene, kStaticMesh, kScript, kNone };

struct AssetInfo {
  UUID id;
  AssetType type;
  std::string meta_path;

  [[nodiscard]] std::string GetAssetPath() const;
};

template <typename T>
struct Asset {
  AssetInfo info;
  Ref<T> asset;

  [[nodiscard]] operator bool() const { return (bool)asset; }
};

template <typename T>
using AssetRef = Ref<Asset<T>>;

}  // namespace eve