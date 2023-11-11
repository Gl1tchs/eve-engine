// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/hierarchy_panel.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

void HierarchyPanel::SetScene(const Ref<Scene>& scene) {
  scene_ = scene;
  selected_entity_ = {};
}

void HierarchyPanel::SetSelectedEntity(Entity entity) {
  selected_entity_ = entity;
}

void HierarchyPanel::Draw() {
  if (!scene_) {
    return;
  }

  auto view = scene_->registry_.view<entt::entity>();

  for (auto entity_id : view) {
    Entity entity{entity_id, scene_.get()};
    const std::string& name = entity.GetName();

    if (ImGui::Selectable(name.c_str(), selected_entity_ == entity)) {
      SetSelectedEntity(entity);
    }
  }
}