// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "project/project_serializer.h"

#include "core/serialization/ini_parser.h"

#include "yaml-cpp/yaml.h"

ProjectSerializer::ProjectSerializer(Ref<Project> project)
    : project_(project) {}

void ProjectSerializer::Serialize(const fs::path& path) {
  const auto& config = project_->GetConfig();

  ini::IniObject obj;

  ini::IniSection project_section{"project", {}};
  project_section.Add("name", config.name);
  project_section.Add("asset_directory", config.asset_directory.string());
  obj.AddSection(project_section);

  ini::IniSection scenes_section{"scenes", {}};
  for (const auto& scene : config.scenes) {
    scenes_section.Add(fs::path(scene).stem().string(), scene);
  }
  obj.AddSection(scenes_section);

  std::ofstream fout(path);
  fout << obj.ToString();
}

bool ProjectSerializer::Deserialize(const fs::path& path) {
  auto& config = project_->GetConfig();

  auto obj = ini::ParseFile(path);
  if (!obj) {
    LOG_ERROR("Failed to load project file '{}'", path.string());
    return false;
  }

  ini::IniSection& project_section = obj.GetSection("project");
  if (!project_section) {
    return false;
  }

  config.name = project_section.values["name"].As<std::string>();
  config.asset_directory =
      project_section.values["asset_directory"].As<fs::path>();

  ini::IniSection& scenes_section = obj.GetSection("scenes");
  if (!scenes_section) {
    return false;
  }

  for (auto& [key, value] : scenes_section.values) {
    config.scenes.push_back(value.As<std::string>());
  }

  return true;
}
