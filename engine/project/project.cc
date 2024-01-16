// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "project/project.h"

#include "asset/asset_registry.h"
#include "core/event/input.h"
#include "core/instance.h"
#include "project/project_serializer.h"
#include "scene/scene_manager.h"

namespace eve {

Ref<Project> Project::active_project_ = nullptr;

Project::Project(const fs::path& path)
    : project_path_(path), project_dir_(path.parent_path()) {}

Ref<Project> Project::New() {
  active_project_ = CreateRef<Project>(fs::path());

  SceneManager::Init(active_project_);
  Input::Init();

  return active_project_;
}

Ref<Project> Project::Load(const fs::path& path) {
  Ref<Project> project = CreateRef<Project>(path);
  project->project_path_ = path;

  ProjectSerializer serializer(project);
  if (serializer.Deserialize(path)) {
    project->project_dir_ = path.parent_path();
    active_project_ = project;

    Input::Init();
    Input::Deserialize(GetKeymapsPath());

    AssetRegistry::Deserialize(GetAssetRegistryPath());
    SceneManager::Init(active_project_);

    return active_project_;
  }

  return nullptr;
}

void Project::SaveActive(const fs::path& path) {
  ProjectSerializer serializer(active_project_);
  serializer.Serialize(path);

  Input::Serialize(GetKeymapsPath());
  AssetRegistry::Serialize(GetAssetRegistryPath());

  active_project_->project_dir_ = path.parent_path();
  active_project_->project_path_ = path;
}

std::string Project::GetProjectName() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->config_.name;
}

fs::path Project::GetProjectDirectory() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->project_dir_;
}

fs::path Project::GetProjectPath() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->project_path_;
}

fs::path Project::GetScriptDirectory() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->project_dir_ /
         active_project_->config_.script_directory;
}

fs::path Project::GetAssetRegistryPath() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->project_dir_ /
         active_project_->config_.asset_registry;
}

fs::path Project::GetKeymapsPath() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->project_dir_ / active_project_->config_.keymaps;
}

// Relative to project directory
fs::path Project::GetAssetDirectory() {
  EVE_ASSERT_ENGINE(active_project_);
  return active_project_->project_dir_ /
         active_project_->config_.asset_directory;
}

}  // namespace eve
