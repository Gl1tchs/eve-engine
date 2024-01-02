// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_manager.h"

#include "core/instance.h"
#include "scene/scene_serializer.h"
#include "scene_manager.h"

namespace eve {
Ref<Project> SceneManager::project_ = nullptr;
SceneManager::ActiveSceneInfo SceneManager::scene_info_{};

void SceneManager::Init(Ref<Project> project) {
  project_ = project;
}

void SceneManager::SetActive(const uint32_t index) {
  // Best code I've written to this day
  ASSERT(DoesIndexExists(index));

  scene_info_.index = index;
  scene_info_.path = project_->GetConfig().scenes[scene_info_.index];

  Ref<Scene> new_scene = CreateRef<Scene>(Instance::Get().GetState());
  SceneSerializer serializer(new_scene);
  ASSERT(serializer.Deserialize(GetActivePath()));

  // If scene is running enque the change to the next frame
  if (scene_info_.scene && scene_info_.scene->IsRunning()) {
    // Perform the scene change on the next frame
    Instance::Get().EnqueueMain([new_scene]() {
      scene_info_.scene->OnRuntimeStop();
      new_scene->OnRuntimeStart();
      scene_info_.scene = new_scene;
    });
  } else {
    scene_info_.scene = new_scene;
  }
}

bool SceneManager::DoesIndexExists(const uint32_t index) {
  return project_->GetConfig().scenes.size() > index;
}

}  // namespace eve
