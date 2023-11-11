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
  AssetRef<T> LoadFromMeta(const std::filesystem::path& path);

  [[nodiscard]] bool Exists(GUUID id);

  [[nodiscard]] bool Exists(const std::string& name);

 private:
  std::unordered_map<std::filesystem::path, AssetInfo> assets_;
};

#include "asset/asset_library.inl"
