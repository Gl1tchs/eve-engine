// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_registry.h"

#include <nlohmann/json.hpp>

#include "asset/asset_loader.h"
#include "asset_registry.h"
#include "project/project.h"

using json = nlohmann::json;

namespace eve {

NLOHMANN_JSON_SERIALIZE_ENUM(AssetType, {{AssetType::kNone, nullptr},
                                         {AssetType::kTexture, "Texture"},
                                         {AssetType::kFont, "Font"},
                                         {AssetType::kStaticMesh, "StaticMesh"},
                                         {AssetType::kScript, "Script"}});

std::unordered_map<AssetHandle, Ref<Asset>> AssetRegistry::assets_ = {};

Ref<Asset> AssetRegistry::Get(const AssetHandle& handle) {
  const auto it = assets_.find(handle);
  if (it == assets_.end()) {
    LOG_ENGINE_WARNING("Asset of handle: {} not found thus cannot load!",
                       (uint64_t)handle);
    return nullptr;
  }

  return it->second;
}

void AssetRegistry::Register(Ref<Asset> asset) {
  assets_[asset->handle] = asset;
}

AssetHandle AssetRegistry::Load(const std::string& path, AssetType type,
                                const std::string& name, AssetHandle handle) {
  const fs::path path_abs = GetAssetPath(path);

  Ref<Asset> asset = nullptr;
  switch (type) {
    case AssetType::kTexture:
      asset = AssetLoader::LoadTexture(path_abs);
      break;
    case AssetType::kStaticMesh:
      asset = AssetLoader::LoadModel(path_abs);
      break;
      break;
    default:
      break;
  }

  if (!asset) {
    LOG_ENGINE_ERROR("Unable to load asset from: {}", path);
    return 0;
  }

  asset->handle = handle;
  asset->name = name;
  asset->path = path;

  assets_[asset->handle] = asset;

  return handle;
}

void AssetRegistry::Unload(const AssetHandle& handle) {
  const auto it = assets_.find(handle);
  if (it == assets_.end()) {
    LOG_ENGINE_WARNING("Asset of handle: {} not found thus cannot remove!",
                       (uint64_t)handle);
    return;
  }

  assets_.erase(it);
}

bool AssetRegistry::Exists(const AssetHandle& handle) {
  return assets_.find(handle) != assets_.end();
}

fs::path AssetRegistry::GetAssetPath(std::string relative_path) {
  std::string proj_substr = "prj://";
  std::string res_substr = "res://";

  if (auto pos = relative_path.find(proj_substr); pos != std::string::npos) {
    auto project_dir = Project::GetProjectDirectory();

    relative_path.erase(pos, proj_substr.length());

    return project_dir / relative_path;
  } else if (auto pos = relative_path.find(res_substr);
             pos != std::string::npos) {
    auto asset_dir = Project::GetAssetDirectory();

    relative_path.erase(pos, res_substr.length());

    return asset_dir / relative_path;
  }

  return relative_path;
}

fs::path AssetRegistry::GetAssetPathTrimmed(std::string relative_path) {
  std::string proj_substr = "prj://";
  std::string res_substr = "res://";

  if (auto pos = relative_path.find(proj_substr); pos != std::string::npos) {
    auto project_dir = Project::GetProjectDirectory();

    relative_path.erase(pos, proj_substr.length());

    return relative_path;
  } else if (auto pos = relative_path.find(res_substr);
             pos != std::string::npos) {
    auto asset_dir = Project::GetAssetDirectory();

    relative_path.erase(pos, res_substr.length());

    return relative_path;
  }

  return relative_path;
}

bool AssetRegistry::Serialize(const fs::path& path) {
  json j{json::array()};

  for (const auto& [handle, asset] : assets_) {
    j.push_back(json{{"handle", (uint64_t)handle},
                     {"name", asset->name},
                     {"path", asset->path},
                     {"type", asset->GetType()}});
  }

  std::ofstream fout(path);
  fout << j.dump(2);

  return true;
}

bool AssetRegistry::Deserialize(const fs::path& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    LOG_ENGINE_ERROR("Unable to deserialize asset registry from: {}",
                     path.string());
    return false;
  }

  json j;
  file >> j;

  if (!j.is_array()) {
    LOG_ENGINE_ERROR("Unable to parse asset registry from: {}", path.string());
    return false;
  }

  for (const auto& asset_json : j) {
    const AssetHandle id = asset_json["handle"].get<uint64_t>();
    const std::string name = asset_json["name"].get<std::string>();
    const std::string path = asset_json["path"].get<std::string>();
    const AssetType type = asset_json["type"].get<AssetType>();

    Load(path, type, name, id);
  }

  return true;
}

}  // namespace eve
