// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "project/project.h"

#include "project/project_serializer.h"

Ref<Project> Project::active_project_ = nullptr;

Ref<Project> Project::New() {
  active_project_ = CreateRef<Project>();
  return active_project_;
}

Ref<Project> Project::Load(const std::filesystem::path& path) {
  Ref<Project> project = CreateRef<Project>();

  ProjectSerializer serializer(project);
  if (serializer.Deserialize(path)) {
    project->project_dir_ = path.parent_path();
    active_project_ = project;
    return active_project_;
  }

  return nullptr;
}

void Project::SaveActive(const std::filesystem::path& path) {
  ProjectSerializer serializer(active_project_);
  serializer.Serialize(path);
  active_project_->project_dir_ = path.parent_path();
}
