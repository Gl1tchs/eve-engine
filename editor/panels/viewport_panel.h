// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "graphics/frame_buffer.h"
#include "scene/editor_camera.h"

#include "panels/hierarchy_panel.h"
#include "panels/panel.h"

namespace eve {
class ViewportPanel : public Panel {
  IMPL_PANEL("Viewport")

 public:
  ViewportPanel(Ref<FrameBuffer>& frame_buffer,
                Ref<HierarchyPanel> hierarchy_panel,
                EditorCamera* editor_camera);

  void SetGizmoState(int state) { operation_ = state; }

  void SetShouldDrawGizmos(bool value) { should_draw_gizmos_ = value; }

 protected:
  void Draw() override;

 private:
  Ref<FrameBuffer> frame_buffer_;
  Ref<HierarchyPanel> hierarchy_panel_;
  EditorCamera* editor_camera_;

  int operation_ = 7;

  bool should_draw_gizmos_ = true;
};
}  // namespace eve
