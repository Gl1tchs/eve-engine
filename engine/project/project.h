// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {
struct ProjectConfig {
  std::string name;
  std::string asset_directory;
  std::string script_directory;
  std::string asset_registry;
  std::string keymaps;
  std::vector<std::string> scenes;
};

class Project {
 public:
  Project(const fs::path& path);

  [[nodiscard]] static std::string GetProjectName();

  [[nodiscard]] static fs::path GetProjectDirectory();

  [[nodiscard]] static fs::path GetProjectPath();

  [[nodiscard]] static fs::path GetScriptDirectory();

  [[nodiscard]] static fs::path GetAssetRegistryPath();

  [[nodiscard]] static fs::path GetKeymapsPath();

  // Relative to project directory
  [[nodiscard]] static fs::path GetAssetDirectory();

  [[nodiscard]] ProjectConfig& GetConfig() { return config_; }

  [[nodiscard]] static Ref<Project> GetActive() { return active_project_; }

  static Ref<Project> New();

  static Ref<Project> Load(const fs::path& path);

  static void SaveActive(const fs::path& path);

  static void SetActive(Ref<Project> project) { active_project_ = project; }

 private:
  static Ref<Project> active_project_;

  ProjectConfig config_;
  fs::path project_dir_;
  fs::path project_path_;

  friend class ProjectSettingsPanel;
};
}  // namespace eve
