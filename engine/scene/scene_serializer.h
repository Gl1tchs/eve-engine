// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/scene.h"

class SceneSerializer {
 public:
  SceneSerializer(const Ref<Scene>& scene);

  void Serialize(const std::filesystem::path& file_path);

  bool Deserialize(const std::filesystem::path& file_path);

 private:
  Ref<Scene> scene_;
};
