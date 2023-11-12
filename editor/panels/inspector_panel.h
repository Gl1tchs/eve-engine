// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "scene/entity.h"

#include "panels/hierarchy_panel.h"
#include "panels/panel.h"

class InspectorPanel : public Panel {
  IMPL_PANEL("Inspector")

 public:
  InspectorPanel(Ref<HierarchyPanel> hierarchy_panel);

  [[nodiscard]] bool IsModified() { return modified_; }
  void SetModified(bool modified) { modified_ = modified; }

 protected:
  void Draw() override;

 private:
  Ref<HierarchyPanel> hierarchy_panel_;
  bool modified_ = false;
};
