// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"

namespace eve {

using AssetRegistryMap = std::unordered_map<AssetHandle, Ref<Asset>>;

class AssetRegistry {
 public:
  AssetRegistry() = default;
  ~AssetRegistry() = default;

  template <typename T>
    requires std::is_base_of_v<Asset, T>
  static Ref<T> Get(const AssetHandle& handle) {
    Ref<Asset> asset = Get(handle);
    return std::static_pointer_cast<T>(asset);
  }

  static Ref<Asset> Get(const AssetHandle& id);

  static void Register(Ref<Asset> asset);

  static AssetHandle Load(const std::string& path, AssetType type,
                          const std::string& name, AssetHandle id = {});

  static void Unload(const AssetHandle& id);

  [[nodiscard]] static bool Exists(const AssetHandle& id);

  // Get the path with some predefined substrings
  // Use "res://" to get asset directory
  // Use "proj://" to get project directory
  [[nodiscard]] static fs::path GetAssetPath(std::string relative_path);

  [[nodiscard]] static fs::path GetAssetPathTrimmed(std::string relative_path);

  [[nodiscard]] static AssetRegistryMap& GetRegistryMap() { return assets_; }

  static bool Serialize(const fs::path& path);
  static bool Deserialize(const fs::path& path);

 private:
  static AssetRegistryMap assets_;
};

}  // namespace eve
