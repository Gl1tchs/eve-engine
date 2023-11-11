// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "project/project.h"

class ProjectSerializer {
 public:
  ProjectSerializer(Ref<Project> project);

  void Serialize(const std::filesystem::path& path);

  bool Deserialize(const std::filesystem::path& path);

 private:
  Ref<Project> project_;
};
