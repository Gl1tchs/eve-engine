// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset_registry.h"
#include "project/project.h"
#include "scene/entity.h"
#include "scene/scene.h"

namespace eve {
class SceneManager {
 public:
  static void Init(Ref<Project> project);

  /**
   * @brief Set the Active scene object based on the build index
   * 
   * @param index Build index of the scene to change
   * @return Sets Active scene object
   */
  static void SetActive(uint32_t index);

  static bool DoesIndexExists(uint32_t index);

  [[nodiscard]] static const uint32_t GetRegisteredSceneCount() {
    return project_->GetConfig().scenes.size();
  }

  [[nodiscard]] static Ref<Scene>& GetActive() { return scene_info_.scene; }

  [[nodiscard]] static const std::string GetActivePath() {
    return AssetRegistry::GetAssetPath(scene_info_.path).string();
  }

  [[nodiscard]] static const uint32_t& GetActiveIndex() {
    return scene_info_.index;
  }

 private:
  static Ref<Project> project_;

  struct ActiveSceneInfo {
    Ref<Scene> scene = nullptr;
    uint32_t index = 0;
    std::string path = "";
  };
  static ActiveSceneInfo scene_info_;
};
}  // namespace eve
