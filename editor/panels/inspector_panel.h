// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "panels/panel.h"

#include "scene/entity.h"

#include "panels/hierarchy_panel.h"

class InspectorPanel : public Panel {
  IMPL_PANEL("Inspector")

 public:
  InspectorPanel(Ref<HierarchyPanel> hierarchy_panel);

  void ResetModified() { modified_ = false; }
  [[nodiscard]] bool IsModified() { return modified_; }

 protected:
  void Draw() override;

 private:
  Ref<HierarchyPanel> hierarchy_panel_;
  bool modified_ = false;
};
