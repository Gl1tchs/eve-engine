// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "project/project_serializer.h"

#include <nlohmann/json.hpp>

#include "asset/asset_registry.h"

using json = nlohmann::json;

namespace eve {

ProjectSerializer::ProjectSerializer(Ref<Project> project)
    : project_(project) {}

void ProjectSerializer::Serialize(const fs::path& path) {
  json j{};

  const auto& config = project_->GetConfig();

  j = json{{"name", config.name},
           {"asset_directory", config.asset_directory},
           {"asset_registry", config.asset_registry},
           {"scenes", json::array()}};

  for (const auto& scene : config.scenes) {
    j["scenes"].push_back(scene);
  }

  std::ofstream fout(path);
  fout << j.dump(2);
}

bool ProjectSerializer::Deserialize(const fs::path& path) {
  auto& config = project_->GetConfig();

  std::ifstream file(path);
  if (!file.is_open()) {
    LOG_ENGINE_ERROR("Failed to load project file from: {}", path.string());
    return false;
  }

  json j;
  file >> j;

  config.name = j["name"].get<std::string>();
  config.asset_directory = j["asset_directory"].get<fs::path>();
  config.asset_registry = j["asset_registry"].get<fs::path>();

  auto scenes_json = j["scenes"];
  for (auto& scene : scenes_json) {
    config.scenes.push_back(scene.get<std::string>());
  }

  return true;
}

}  // namespace eve
