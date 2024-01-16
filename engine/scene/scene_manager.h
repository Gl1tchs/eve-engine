// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

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

  [[nodiscard]] static const uint32_t GetRegisteredSceneCount();

  [[nodiscard]] static Ref<Scene>& GetActive();

  [[nodiscard]] static const std::string GetActivePath();

  [[nodiscard]] static const uint32_t& GetActiveIndex();
};

}  // namespace eve
