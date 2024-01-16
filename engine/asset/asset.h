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

/**
 * @brief Enum representing an asset type
 * to keep assets in order with each other.
 */
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

/**
 * @brief Try to find which asset type associated with which type.
 * 
 * @param extension Filetype extension in a format of @c ".extension"
 * @return AssetType The type if founc @c AssetType::kNone otherwise.
 */
AssetType GetAssetTypeFromExtension(const std::string& extension);

/**
 * @brief Deserialize @c AssetType enum into string.
 * 
 * @param type AssetType to deserialize.
 * @return std::string Deserialized string.
 */
std::string DeserializeAssetType(AssetType type);

/**
 * @brief Abstract class representing an asset
 * in asset registry.
 */
struct Asset {
  AssetHandle handle = 0;
  std::string name = "";
  std::string path = "";

  /**
   * @brief Which type associated with the asset.
   * 
   * @return constexpr AssetType Type of the asset
   */
  virtual constexpr AssetType GetType() const = 0;
};

}  // namespace eve