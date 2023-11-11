// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>

#include "asset/asset_library.h"
#include "scene/scene.h"

class SceneSerializer {
 public:
  SceneSerializer(const Ref<Scene>& scene,
                  const Ref<AssetLibrary>& asset_library);

  void Serialize(const std::filesystem::path& file_path);

  bool Deserialize(const std::filesystem::path& file_path);

 private:
  Ref<Scene> scene_;
  Ref<AssetLibrary> asset_library_;
};
