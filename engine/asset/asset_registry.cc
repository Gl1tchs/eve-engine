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
                                         {AssetType::kScript, "Script"},
                                         {AssetType::kMaterial, "Material"},
                                         {AssetType::kShader, "Shader"}});

struct AssetMetadata {
  std::string name;
  std::string path;
  AssetType type = AssetType::kNone;
};

AssetMetadata GetMetadata(const fs::path& path) {
  AssetMetadata metadata;
  metadata.name = path.stem().string();
  metadata.path = path.string();
  metadata.type = GetAssetTypeFromExtension(path.extension().string());

  return metadata;
}

std::unordered_map<AssetHandle, Ref<Asset>> AssetRegistry::assets_ = {};

Ref<Asset> AssetRegistry::Get(const AssetHandle& handle) {
  const auto it = assets_.find(handle);
  if (it == assets_.end()) {
    EVE_LOG_ENGINE_WARNING("Asset of handle: {} not found thus cannot load!",
                           (uint64_t)handle);
    return nullptr;
  }

  return it->second;
}

AssetHandle AssetRegistry::GetFromPath(const std::string& path) {
  const AssetRegistryMap::const_iterator it = std::find_if(
      assets_.begin(), assets_.end(),
      [path](const auto& pair) { return path == pair.second->path; });

  // If not exists try to add
  if (it == assets_.end()) {
    EVE_LOG_ENGINE_ERROR("Unable to get asset from path: {}", path);
    return 0;
  }

  return it->first;
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
      asset = asset_loader::LoadTexture(path_abs);
      break;
    case AssetType::kStaticMesh:
      asset = asset_loader::LoadModel(path_abs);
      break;
    case AssetType::kShader:
      asset = asset_loader::LoadShader(path_abs);
      break;
    default:
      return 0;
  }

  if (!asset) {
    EVE_LOG_ENGINE_ERROR("Unable to load asset from: {}", path);
    return 0;
  }

  asset->handle = handle;
  asset->name = name;
  asset->path = path;

  assets_[asset->handle] = asset;

  return handle;
}

AssetHandle AssetRegistry::Load(const std::string& path) {
  const AssetMetadata asset = GetMetadata(path);

  if (AssetHandle handle = GetFromPath(path); handle != 0) {
    return handle;
  }

  if (AssetHandle handle =
          AssetRegistry::Load(asset.path, asset.type, asset.name);
      handle) {
    return handle;
  } else {
    EVE_LOG_EDITOR_ERROR("Unable to load asset from: {}", asset.path);
    return 0;
  }
}

bool AssetRegistry::Unload(const AssetHandle& handle) {
  const auto it = assets_.find(handle);
  if (it == assets_.end()) {
    EVE_LOG_ENGINE_WARNING("Asset of handle: {} not found thus cannot remove!",
                           (uint64_t)handle);
    return false;
  }

  assets_.erase(it);

  return true;
}

bool AssetRegistry::Reload(Ref<Asset>& asset) {
  const AssetHandle handle = asset->handle;
  const std::string name = asset->name;
  const std::string path = asset->path;
  const AssetType type = asset->GetType();

  if (!Unload(asset->handle)) {
    return false;
  }

  Load(path, type, name, handle);

  Ref<Asset> new_asset = Get(handle);
  if (!new_asset) {
    return false;
  }

  asset = new_asset;

  return true;
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

std::string AssetRegistry::GetRelativePath(const std::string& path) {
  auto project_dir = Project::GetProjectDirectory();
  auto asset_dir = Project::GetAssetDirectory();

  fs::path relative_path;

  // Check if the path is inside the asset directory
  if (path.compare(0, asset_dir.string().length(), asset_dir.string()) == 0) {
    relative_path = fs::relative(path, asset_dir);
    return "res://" + relative_path.string();
  }

  // Check if the path is inside the project directory
  if (path.compare(0, project_dir.string().length(), project_dir.string()) ==
      0) {
    relative_path = fs::relative(path, project_dir);
    return "prj://" + relative_path.string();
  }

  // If the path is not inside the project or asset directory, return the original path
  return path;
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
  assets_.clear();

  std::ifstream file(path);
  if (!file.is_open()) {
    EVE_LOG_ENGINE_ERROR("Unable to deserialize asset registry from: {}",
                         path.string());
    return false;
  }

  json j;
  file >> j;

  if (!j.is_array()) {
    EVE_LOG_ENGINE_ERROR("Unable to parse asset registry from: {}",
                         path.string());
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
