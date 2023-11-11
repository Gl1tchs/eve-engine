// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_loader.h"

#include <yaml-cpp/yaml.h>

#include "asset/asset_library.h"
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

AssetRef<Texture> AssetLoader::LoadTexture(const std::string& path) {
  std::string path_absolute = AssetLibrary::GetAssetPath(path).string();

  YAML::Node data;
  try {
    data = YAML::LoadFile(path_absolute);
  } catch (YAML::ParserException e) {
    LOG_ENGINE_ERROR("Failed to load asset from file '{0}'\n\t{1}",
                     path_absolute, e.what());
    return {};
  }

  if (!data["type"] || !data["path"]) {
    LOG_ENGINE_ERROR("Failed to load asset metadata.");
    return {};
  }

  std::string asset_path = data["path"].as<std::string>();

  AssetType type = static_cast<AssetType>(data["type"].as<int>());
  if (type != AssetType::kTexture) {
    LOG_ENGINE_ERROR(
        "Failed to load asset from: {0}\nAsset types did not match with "
        "texture!",
        path);
    return {};
  }

  AssetRef<Texture> texture = CreateRef<Asset<Texture>>();

  AssetInfo info;

  info.type = AssetType::kTexture;

  if (data["name"]) {
    info.name = data["name"].as<std::string>();
  }

  if (data["id"]) {
    info.id = GUUID(data["id"].as<uint64_t>());
  } else {
    info.id = GUUID();
  }

  info.meta_path = path;

  TextureMetadata metadata;
  if (auto metadata_yaml = data["texture_metadata"]; metadata_yaml) {
    if (metadata_yaml["format"]) {
      metadata.format =
          static_cast<TextureFormat>(metadata_yaml["format"].as<int>());
    }

    if (metadata_yaml["size"]) {
      metadata.size = metadata_yaml["size"].as<glm::ivec2>();
    }

    if (metadata_yaml["min_filter"]) {
      metadata.min_filter = static_cast<TextureFilteringMode>(
          metadata_yaml["min_filter"].as<int>());
    }

    if (metadata_yaml["mag_filter"]) {
      metadata.mag_filter = static_cast<TextureFilteringMode>(
          metadata_yaml["mag_filter"].as<int>());
    }

    if (metadata_yaml["wrap_s"]) {
      metadata.wrap_s =
          static_cast<TextureWrappingMode>(metadata_yaml["wrap_s"].as<int>());
    }

    if (metadata_yaml["wrap_t"]) {
      metadata.wrap_t =
          static_cast<TextureWrappingMode>(metadata_yaml["wrap_t"].as<int>());
    }

    if (metadata_yaml["generate_mipmaps"]) {
      metadata.generate_mipmaps = metadata_yaml["generate_mipmaps"].as<bool>();
    }
  }

  texture->info = info;
  texture->asset = Texture::Create(asset_path, metadata);

  return texture;
}
