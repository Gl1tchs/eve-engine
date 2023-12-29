// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

struct ProjectConfig {
  std::string name;
  fs::path asset_directory;
  std::vector<std::string> scenes;
};

class Project {
 public:
  [[nodiscard]] static fs::path GetProjectDirectory() {
    ASSERT(active_project_)
    return active_project_->project_dir_;
  }

  // Relative to project directory
  [[nodiscard]] static fs::path GetAssetDirectory() {
    ASSERT(active_project_)
    using std::filesystem::path;
    return (path(active_project_->project_dir_) /
            path(active_project_->config_.asset_directory))
        .string();
  }

  [[nodiscard]] ProjectConfig& GetConfig() { return config_; }

  [[nodiscard]] static Ref<Project> GetActive() { return active_project_; }

  static Ref<Project> New();

  static Ref<Project> Load(const fs::path& path);

  static void SaveActive(const fs::path& path);

 private:
  static Ref<Project> active_project_;

  ProjectConfig config_;
  fs::path project_dir_;
};
