// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_loader.h"

#include <yaml-cpp/yaml.h>

#include "asset/asset_library.h"
#include "asset_loader.h"
#include "core/debug/log.h"

namespace YAML {

template <>
struct convert<glm::ivec2> {
  static Node encode(const glm::ivec2& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::ivec2& rhs) {
    if (!node.IsSequence() || node.size() != 2)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    return true;
  }
};

template <>
struct convert<TextureMetadata> {
  static Node encode(const TextureMetadata& rhs) {
    Node node;
    node["size"] = rhs.size;
    node["format"] = static_cast<int>(rhs.format);
    node["min_filter"] = static_cast<int>(rhs.min_filter);
    node["mag_filter"] = static_cast<int>(rhs.mag_filter);
    node["wrap_s"] = static_cast<int>(rhs.wrap_s);
    node["wrap_t"] = static_cast<int>(rhs.wrap_t);
    node["generate_mipmaps"] = rhs.generate_mipmaps;
    return node;
  }

  static bool decode(const Node& node, TextureMetadata& rhs) {
    if (!node["size"] || !node["format"] || !node["min_filter"] ||
        !node["mag_filter"] || !node["wrap_s"] || !node["wrap_t"] ||
        !node["generate_mipmaps"]) {
      return false;
    }

    rhs.size = node["size"].as<glm::ivec2>();
    rhs.format = static_cast<TextureFormat>(node["format"].as<int>());
    rhs.min_filter =
        static_cast<TextureFilteringMode>(node["min_filter"].as<int>());
    rhs.mag_filter =
        static_cast<TextureFilteringMode>(node["mag_filter"].as<int>());
    rhs.wrap_s = static_cast<TextureWrappingMode>(node["wrap_s"].as<int>());
    rhs.wrap_t = static_cast<TextureWrappingMode>(node["wrap_t"].as<int>());
    rhs.generate_mipmaps = node["generate_mipmaps"].as<bool>();

    return true;
  }
};

}  // namespace YAML

struct AssetInfoYAML {
  std::string asset_path;
  GUUID id;
  YAML::Node metadata;
};

[[nodiscard]] static std::optional<AssetInfoYAML> DeserializeAssetInfo(
    const std::string& path) {
  std::string absolute_path = AssetLibrary::GetAssetPath(path).string();
  if (std::filesystem::path(absolute_path).extension() != ".meta") {
    LOG_ENGINE_ERROR("Unable to load .meta file extension not valid.");
    return {};
  }

  YAML::Node data;
  try {
    data = YAML::LoadFile(absolute_path);
  } catch (YAML::ParserException e) {
    LOG_ENGINE_ERROR("Failed to load asset from file '{0}'\n\t{1}",
                     absolute_path, e.what());
    return {};
  }

  if (!data["path"]) {
    LOG_ENGINE_ERROR("Failed to load asset metadata.");
    return {};
  }

  AssetInfoYAML info;

  info.asset_path = data["path"].as<std::string>();

  if (data["id"]) {
    info.id = GUUID(data["id"].as<uint64_t>());
  } else {
    info.id = GUUID();
  }

  info.metadata = data["metadata"];

  return std::optional<AssetInfoYAML>(info);
}

AssetRef<Texture> AssetLoader::LoadTexture(const std::string& path) {
  std::optional<AssetInfoYAML> info_yaml = DeserializeAssetInfo(path);
  if (!info_yaml.has_value()) {
    return nullptr;
  }

  TextureMetadata texture_metadata;
  if (auto metadata_yaml = info_yaml.value().metadata["texture_metadata"];
      metadata_yaml) {
    if (metadata_yaml["format"]) {
      texture_metadata.format =
          static_cast<TextureFormat>(metadata_yaml["format"].as<int>());
    }
    if (metadata_yaml["size"]) {
      texture_metadata.size = metadata_yaml["size"].as<glm::ivec2>();
    }
    if (metadata_yaml["min_filter"]) {
      texture_metadata.min_filter = static_cast<TextureFilteringMode>(
          metadata_yaml["min_filter"].as<int>());
    }
    if (metadata_yaml["mag_filter"]) {
      texture_metadata.mag_filter = static_cast<TextureFilteringMode>(
          metadata_yaml["mag_filter"].as<int>());
    }
    if (metadata_yaml["wrap_s"]) {
      texture_metadata.wrap_s =
          static_cast<TextureWrappingMode>(metadata_yaml["wrap_s"].as<int>());
    }
    if (metadata_yaml["wrap_t"]) {
      texture_metadata.wrap_t =
          static_cast<TextureWrappingMode>(metadata_yaml["wrap_t"].as<int>());
    }
    if (metadata_yaml["generate_mipmaps"]) {
      texture_metadata.generate_mipmaps =
          metadata_yaml["generate_mipmaps"].as<bool>();
    }
  }

  AssetInfo info;
  info.meta_path = path;
  info.type = AssetType::kTexture;
  info.id = info_yaml.value().id;

  AssetRef<Texture> texture = CreateRef<Asset<Texture>>();
  texture->info = info;
  texture->asset =
      Texture::Create(AssetLibrary::GetAssetPath(info_yaml.value().asset_path),
                      texture_metadata);

  return texture;
}

AssetRef<Model> AssetLoader::LoadModel(const std::string& path) {
  std::optional<AssetInfoYAML> info_yaml = DeserializeAssetInfo(path);
  if (!info_yaml.has_value()) {
    return nullptr;
  }

  AssetRef<Model> model = CreateRef<Asset<Model>>();

  AssetInfo info;
  info.meta_path = path;
  info.type = AssetType::kStaticMesh;
  info.id = info_yaml.value().id;

  model->info = info;
  model->asset =
      Model::Load(AssetLibrary::GetAssetPath(info_yaml.value().asset_path));

  return model;
}
