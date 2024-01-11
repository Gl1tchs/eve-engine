// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/state.h"
#include "graphics/skybox.h"
#include "scene/editor_camera.h"

namespace eve {
class SceneRenderer {
 public:
  SceneRenderer(const Ref<State>& state);

  void RenderRuntime(float ds);

  void RenderEditor(float ds, Ref<EditorCamera>& editor_camera,
                    bool use_primary_if_exists = false);

  void OnViewportResize(glm::uvec2 size);

 private:
  void RenderSceneEditor(const CameraData& data);

  void RenderSceneRuntime(const CameraData& data);

  void RenderScene();

  void DrawGrid();

  void RenderCameraBounds();

 private:
  Ref<State> state_;

  Ref<SkyBox> skybox_;

  glm::uvec2 viewport_size_;

  bool editor_primary_used_ = false;
  Transform last_primary_transform_;
};
}  // namespace eve