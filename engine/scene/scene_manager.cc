// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_manager.h"

#include "asset/asset_registry.h"
#include "core/instance.h"
#include "scene/scene_serializer.h"

namespace eve {

struct SceneManagerData {
  Ref<Project> project = nullptr;

  Ref<Scene> active_scene = nullptr;
  uint32_t active_index = 0;
  std::string active_path = "";
};

static SceneManagerData scene_data = {};

void SceneManager::Init(Ref<Project> project) {
  scene_data.project = project;
  scene_data.active_scene = {};
  scene_data.active_index = 0;
  scene_data.active_path = "";
}

void SceneManager::SetActive(const uint32_t index) {
  EVE_ASSERT_ENGINE(DoesIndexExists(index));

  scene_data.active_index = index;
  scene_data.active_path =
      scene_data.project->GetConfig().scenes[scene_data.active_index];

  Ref<Scene> new_scene = CreateRef<Scene>(Instance::Get().GetState());
  SceneSerializer serializer(new_scene);
  EVE_ASSERT_ENGINE(serializer.Deserialize(GetActivePath()));

  // If scene is running enque the change to the next frame
  if (scene_data.active_scene && scene_data.active_scene->IsRunning()) {
    // Perform the scene change on the next frame
    Instance::Get().EnqueueMain([new_scene]() {
      scene_data.active_scene->OnRuntimeStop();
      new_scene->OnRuntimeStart();
      scene_data.active_scene = new_scene;
    });
  } else {
    scene_data.active_scene = new_scene;
  }
}

bool SceneManager::DoesIndexExists(const uint32_t index) {
  return scene_data.project->GetConfig().scenes.size() > index;
}

const uint32_t SceneManager::GetRegisteredSceneCount() {
  return scene_data.project->GetConfig().scenes.size();
}

Ref<Scene>& SceneManager::GetActive() {
  return scene_data.active_scene;
}

const std::string SceneManager::GetActivePath() {
  return AssetRegistry::GetAssetPath(scene_data.active_path).string();
}

const uint32_t& SceneManager::GetActiveIndex() {
  return scene_data.active_index;
}

}  // namespace eve
