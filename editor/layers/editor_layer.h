// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/debug/log.h"
#include "core/layer.h"
#include "graphics/frame_buffer.h"
#include "scene/editor_camera.h"
#include "scene/scene.h"

#include "panels/render_stats_panel.h"
#include "panels/viewport_panel.h"

enum class SceneState { kEdit, kPlay };

class EditorLayer : public Layer {
 public:
  EditorLayer(Ref<State>& state);
  ~EditorLayer();

 private:
  void OnStart() override;

  void OnDestroy() override;

  void OnUpdate(float ds) override;

  void OnGUI(float ds) override;

 private:
  Ref<LogInstance> editor_logger_;

  Ref<Scene> active_scene_;
  SceneState scene_state_ = SceneState::kEdit;

  EditorCamera editor_camera_;

  Ref<FrameBuffer> frame_buffer_;

  Scope<ViewportPanel> viewport_;

  Scope<RenderStatsPanel> render_stats_;

  bool show_stats_{false};
};
