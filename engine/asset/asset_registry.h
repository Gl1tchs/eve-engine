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

  /**
   * @brief Get asset as it's original type.
   * 
   * @tparam T Type of asset to retrieve
   * @param handle Id of the asset to retrieve.
   * @return requires @c asset if found @c nullptr otherwise.
   */
  template <typename T>
    requires std::is_base_of_v<Asset, T>
  static Ref<T> Get(const AssetHandle& handle) {
    if (handle == 0) {
      return nullptr;
    }

    Ref<Asset> asset = Get(handle);
    return std::static_pointer_cast<T>(asset);
  }

  /**
   * @brief Get base asset from id.
   * 
   * @param id Id of the asset to retrieve
   * @return Ref<Asset> @c Asset if exists @c nullptr otherwise.
   */
  [[nodiscard]] static Ref<Asset> Get(const AssetHandle& id);

  /**
   * @brief Get base asset from path.
   * 
   * @param path Path of the asset to retrieve.
   * @return AssetHandle @c id of the asset if found @c AssetHandle(0) otherwise.
   */
  [[nodiscard]] static AssetHandle GetFromPath(const std::string& path);

  /**
   * @brief Register asset object to the asset registry.
   * 
   * @param asset Asset to register.
   */
  static void Register(Ref<Asset> asset);

  /**
   * @brief Load and register an asset to the asset registry.
   * 
   * @param path Path of the asset.
   * @param type Type of the asset.
   * @param name Name of the asset.
   * @param id Id of the asset default will create an unique id.
   * @return AssetHandle Id of the newly registered asset.
   */
  static AssetHandle Load(const std::string& path, AssetType type,
                          const std::string& name, AssetHandle id = {});

  /**
   * @brief Load and register an asset to the registry.
   * This will try to find @b name and @b type from file
   * and it's extension. 
   * 
   * @param path Path of the asset.
   * @return AssetHandle Id of the newly registered asset.
   */
  static AssetHandle Load(const std::string& path);

  /**
   * @brief Removes asset from the asset registry.
   * 
   * @param id Id of the asset to remove.
   * @return true If asset found and removed. 
   * @return false If asset could not found or could not removed.
   */
  static bool Unload(const AssetHandle& id);

  /**
   * @brief Replace an asset's content with new one.
   * 
   * @param asset New asset to replace for.
   * @return true If asset found and replaced successfully.
   * @return false If asset could not found or could not replaced.
   */
  static bool Reload(Ref<Asset>& asset);

  [[nodiscard]] static bool Exists(const AssetHandle& id);

  /**
   * @brief Get the path with some predefined substrings.
   * Use @c res:// to get asset directory.
   * Use @c proj:// to get project directory.
   * 
   * @param relative_path Path to retrieve relatively. 
   * @return fs::path Absolute path of the given relative one.
   */
  [[nodiscard]] static fs::path GetAssetPath(std::string relative_path);

  /**
   * @brief Get relative path without the @c res:// or @c proj:// specifiers.
   * 
   * @param relative_path Path to trim.
   * @return fs::path Trimmed path.
   */
  [[nodiscard]] static fs::path GetAssetPathTrimmed(std::string relative_path);

  /**
   * @brief Turns absolute path to relative asset path.
   * @example @code {.cpp}
   * const std::string asset_path = "~/my_project/assets/textures/my_texture.png"
   * const relative_path = eve::AssetRegistry::GetRelativePath(asset_path);
   * EVE_ENGINE_ASSERT(relative_path == "res://textures/my_texture.png");
   * @endcode
   * 
   * @param path Path to turn relative.
   * @return std::string The relative path.
   */
  [[nodiscard]] static std::string GetRelativePath(const std::string& path);

  [[nodiscard]] static AssetRegistryMap& GetRegistryMap() { return assets_; }

  static bool Serialize(const fs::path& path);

  static bool Deserialize(const fs::path& path);

 private:
  static AssetRegistryMap assets_;
};

}  // namespace eve
