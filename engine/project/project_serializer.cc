// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "project/project_serializer.h"

#include <yaml-cpp/yaml.h>

ProjectSerializer::ProjectSerializer(Ref<Project> project)
    : project_(project) {}

void ProjectSerializer::Serialize(const fs::path& path) {
  const auto& config = project_->GetConfig();

  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "project" << YAML::Value;
  out << YAML::BeginMap;

  out << YAML::Key << "name" << YAML::Value << config.name;
  out << YAML::Key << "asset_directory" << YAML::Value
      << config.asset_directory.string();

  out << YAML::Key << "scenes" << YAML::Value << YAML::BeginSeq;
  for (const auto& scene : config.scenes) {
    out << scene;
  }
  out << YAML::EndSeq;

  out << YAML::EndMap;
  out << YAML::EndMap;

  std::ofstream fout(path);
  fout << out.c_str();
}

bool ProjectSerializer::Deserialize(const fs::path& path) {
  auto& config = project_->GetConfig();

  YAML::Node data;
  try {
    data = YAML::LoadFile(path.string());
  } catch (YAML::ParserException e) {
    LOG_ERROR("Failed to load project file '{0}'\n\t{1}", path.string(),
              e.what());
    return false;
  }

  auto project_node = data["project"];
  if (!project_node)
    return false;

  config.name = project_node["name"].as<std::string>();
  config.asset_directory = project_node["asset_directory"].as<std::string>();
  config.scenes = project_node["scenes"].as<std::vector<std::string>>();
  return true;
}
