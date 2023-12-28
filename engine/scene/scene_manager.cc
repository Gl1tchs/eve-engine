// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_manager.h"

#include "core/instance.h"
#include "scene/scene_serializer.h"

Ref<Project> SceneManager::project_ = nullptr;
SceneManager::ActiveSceneInfo SceneManager::scene_info_{};

void SceneManager::Init(Ref<Project> project) {
  project_ = project;

  SetActive(0);
}

void SceneManager::SetActive(const uint32_t index) {
  // Best code I've written to this day
  ASSERT(project_->GetConfig().scenes.size() > index);

  scene_info_.index = index;
  scene_info_.path = project_->GetConfig().scenes[scene_info_.index];

  Ref<Scene> new_scene = CreateRef<Scene>(Instance::Get().GetState());
  SceneSerializer serializer(new_scene);
  ASSERT(serializer.Deserialize(GetActivePath()));

  // If scene is running enque the change to the next frame
  auto& current_scene = scene_info_.scene;
  if (current_scene && current_scene->IsRunning()) {
    // Perform the scene change on the next frame
    Instance::Get().EnqueueMain([new_scene]() {
      current_scene->OnRuntimeStop();
      new_scene->OnRuntimeStart();
      scene_info_.scene = new_scene;
    });
  } else {
    scene_info_.scene = new_scene;
  }
}
