// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>
#include <string>

#include "core/debug/assert.h"
#include "core/utils/memory.h"

struct ProjectConfig {
  std::string name;
  std::filesystem::path default_scene;
  std::filesystem::path asset_directory;
};

class Project {
 public:
  [[nodiscard]] static std::filesystem::path GetProjectDirectory() {
    ENGINE_ASSERT(active_project_)
    return active_project_->project_dir_;
  }

  // Relative to project directory
  [[nodiscard]] static std::filesystem::path GetAssetDirectory() {
    ENGINE_ASSERT(active_project_)
    return active_project_->project_dir_ /
           active_project_->config_.asset_directory;
  }

  [[nodiscard]] ProjectConfig& GetConfig() { return config_; }

  [[nodiscard]] static Ref<Project> GetActive() { return active_project_; }

  static Ref<Project> New();

  static Ref<Project> Load(const std::filesystem::path& path);

  static void SaveActive(const std::filesystem::path& path);

 private:
  static Ref<Project> active_project_;

  ProjectConfig config_;
  std::filesystem::path project_dir_;
};
