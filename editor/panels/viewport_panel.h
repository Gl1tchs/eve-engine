// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/utils/memory.h"
#include "graphics/frame_buffer.h"

#include "panels/hierarchy_panel.h"
#include "panels/panel.h"
#include "scene/editor_camera.h"

class ViewportPanel : public Panel {
  IMPL_PANEL("Viewport")

 public:
  ViewportPanel(Ref<FrameBuffer>& frame_buffer,
                Ref<HierarchyPanel> hierarchy_panel,
                EditorCamera* editor_camera);

  void SetGizmoState(int state) { operation_ = state; }

 protected:
  void Draw() override;

 private:
  Ref<FrameBuffer> frame_buffer_;
  Ref<HierarchyPanel> hierarchy_panel_;
  EditorCamera* editor_camera_;

  int operation_ = 7;
};
