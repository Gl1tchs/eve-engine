// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "graphics/frame_buffer.h"
#include "scene/editor_camera.h"

#include "panels/hierarchy_panel.h"
#include "ui/panel.h"

namespace eve {

enum class SceneState { kEdit, kPlay, kPaused };

class ViewportPanel : public Panel {
  EVE_IMPL_PANEL("Viewport")

 public:
  std::function<void()> on_play;
  std::function<void()> on_stop;
  std::function<void()> on_pause;
  std::function<void()> on_resume;
  std::function<void()> on_step;

  ViewportPanel(Ref<FrameBuffer>& frame_buffer,
                Ref<HierarchyPanel> hierarchy_panel,
                Ref<EditorCamera> editor_camera);

  void SetGizmoState(int state) { operation_ = state; }

  void SetShouldDrawGizmos(bool value) { should_draw_gizmos_ = value; }

  void SetState(SceneState state) { state_ = state; }

 protected:
  void Draw() override;

  void DrawFrameBufferImage();

  void DrawUIToolbar();

  void DrawGizmos();

 private:
  Ref<FrameBuffer> frame_buffer_;
  Ref<HierarchyPanel> hierarchy_panel_;
  Ref<EditorCamera> editor_camera_;

  SceneState state_ = SceneState::kEdit;

  int operation_ = 7;

  bool should_draw_gizmos_ = true;
};

}  // namespace eve
