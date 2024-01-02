// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "project/project.h"

#include "project/project_serializer.h"
#include "scene/scene_manager.h"

namespace eve {
Ref<Project> Project::active_project_ = nullptr;

Ref<Project> Project::New() {
  active_project_ = CreateRef<Project>();
  
  SceneManager::Init(active_project_);

  return active_project_;
}

Ref<Project> Project::Load(const fs::path& path) {
  Ref<Project> project = CreateRef<Project>();
  project->project_path_ = path;

  ProjectSerializer serializer(project);
  if (serializer.Deserialize(path)) {
    project->project_dir_ = path.parent_path();
    active_project_ = project;

    AssetRegistry::Deserialize(GetAssetRegistryPath());

    SceneManager::Init(active_project_);

    return active_project_;
  }

  return nullptr;
}

void Project::SaveActive(const fs::path& path) {
  ProjectSerializer serializer(active_project_);
  serializer.Serialize(path);

  AssetRegistry::Serialize(GetAssetRegistryPath());

  active_project_->project_dir_ = path.parent_path();
  active_project_->project_path_ = path;
}

}  // namespace eve
