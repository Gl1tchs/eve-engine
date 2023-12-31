// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "scene/entity.h"

#include "panels/hierarchy_panel.h"
#include "panels/panel.h"

namespace eve {
class InspectorPanel : public Panel {
  IMPL_PANEL("Inspector")

 public:
  InspectorPanel(Ref<HierarchyPanel> hierarchy_panel);

  void ToggleAdvanced() { advanced_ = !advanced_; }

 protected:
  void Draw() override;

 private:
  void RenderEntityHeader(Entity selected_entity);

  void RenderComponentProperties(Entity selected_entity);

 private:
  Ref<HierarchyPanel> hierarchy_panel_;

  bool advanced_ = false;
};
}  // namespace eve
