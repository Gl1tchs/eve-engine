// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>
#include <string>

#include "core/utils/guuid.h"
#include "core/utils/memory.h"

enum class AssetType { kTexture, kFont, kScene, kStaticMesh };

struct AssetInfo {
  GUUID id;
  std::string name = "";
  AssetType type;
  std::filesystem::path meta_path;
};

template <typename T>
struct Asset {
  AssetInfo info;
  Ref<T> asset;

  [[nodiscard]] operator bool() const { return (bool)asset; }
};

template <typename T>
using AssetRef = Ref<Asset<T>>;
