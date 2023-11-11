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
  AssetLibrary() = default;
  ~AssetLibrary() = default;

  template <typename T>
  AssetRef<T> LoadFromMeta(const std::string& path);

  [[nodiscard]] bool Exists(GUUID id);

  [[nodiscard]] bool Exists(const std::string& name);

  // Get the path with some predefined substrings
  // Use "res://" to get asset directory
  // Use "proj://" to get project directory
  [[nodiscard]] static std::filesystem::path GetAssetPath(
      std::string relative_path);

 private:
  std::unordered_map<std::string, AssetInfo> assets_;
};

#include "asset/asset_library.inl"
