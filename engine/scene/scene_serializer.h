// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/scene.h"

namespace eve {
class SceneSerializer {
 public:
  SceneSerializer(const Ref<Scene>& scene);

  void Serialize(const fs::path& file_path);

  bool Deserialize(const fs::path& file_path);

 private:
  Ref<Scene> scene_;
};
}  // namespace eve
