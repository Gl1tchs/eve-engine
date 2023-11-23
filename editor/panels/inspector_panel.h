// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "scene/entity.h"

#include "modals/importer_modal.h"
#include "panels/hierarchy_panel.h"
#include "panels/panel.h"

class InspectorPanel : public Panel {
  IMPL_PANEL("Inspector")

 public:
  InspectorPanel(Ref<HierarchyPanel> hierarchy_panel);

 protected:
  void Draw() override;

 private:
  void RenderAddComponentDialog(Entity selected_entity);

  void RenderComponentProperties(Entity selected_entity);

  void OnModelMetaWrite(const std::string& meta_path);

 private:
  Ref<HierarchyPanel> hierarchy_panel_;

  ImporterModal model_importer_;

  bool show_add_component_dialog_ = false;
  bool add_component_dialog_opened_ = false;
};
