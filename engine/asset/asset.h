// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"

namespace eve {

enum class AssetType { kTexture, kFont, kScene, kStaticMesh, kScript, kNone };

template <typename T>
struct Asset {
  UUID id;
  Ref<T> asset;
  std::string path;

  [[nodiscard]] operator bool() const { return (bool)asset; }
};

template <typename T>
using AssetRef = Ref<Asset<T>>;

}  // namespace eve