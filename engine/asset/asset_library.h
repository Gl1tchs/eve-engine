// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <any>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "asset/asset.h"
#include "asset/asset_loader.h"
#include "core/debug/log.h"

class AssetLibrary {
 public:
  template <typename T>
  [[nodiscard]] static AssetRef<T> LoadFromPath(const std::string& path);

  template <typename T>
  [[nodiscard]] static AssetRef<T> LoadFromMeta(const std::string& path);

  // Get the path with some predefined substrings
  // Use "res://" to get asset directory
  // Use "proj://" to get project directory
  [[nodiscard]] static std::filesystem::path GetAssetPath(
      std::string relative_path);
};

#include "asset/asset_library.inl"
