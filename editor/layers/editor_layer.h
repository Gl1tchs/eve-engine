// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/debug/log.h"
#include "core/layer.h"
#include "graphics/frame_buffer.h"
#include "scene/editor_camera.h"
#include "scene/scene.h"

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

  bool viewport_focused_ = false, viewport_hovered_ = false;
  glm::vec2 viewport_size_ = {0.0f, 0.0f};
  glm::vec2 viewport_bounds_[2];

  bool show_stats_{false};
};
