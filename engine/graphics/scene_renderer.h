// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/state.h"
#include "graphics/skybox.h"
#include "scene/editor_camera.h"

namespace eve {

struct SceneRendererSettings {
  bool draw_grid = false;
  bool render_physics_bounds = false;
};

class SceneRenderer {
 public:
  SceneRenderer(const Ref<State>& state);

  void RenderRuntime(float ds);

  void RenderEditor(float ds, Ref<EditorCamera>& editor_camera);

  void OnViewportResize(glm::uvec2 size);

  SceneRendererSettings& GetSettings() { return settings_; }

 private:
  void RenderSceneEditor(const CameraData& data);

  void RenderSceneRuntime(const CameraData& data);

  void RenderScene();

  void DrawGrid();

  void RenderCameraBounds();

  void RenderColliderBounds();

 private:
  SceneRendererSettings settings_;

  Ref<State> state_;
  Ref<SkyBox> skybox_;

  glm::uvec2 viewport_size_;
};

}  // namespace eve