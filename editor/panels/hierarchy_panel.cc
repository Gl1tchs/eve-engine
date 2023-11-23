// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/hierarchy_panel.h"

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "utils/imgui_utils.h"
#include "utils/modify_info.h"

HierarchyPanel::HierarchyPanel() : scene_(nullptr), selected_entity_({}) {}

void HierarchyPanel::SetScene(const Ref<Scene>& scene) {
  scene_ = scene;
  selected_entity_ = Entity{};
}

void HierarchyPanel::SetSelectedEntity(Entity entity) {
  selected_entity_ = entity;
  scene_->selected_entity_ = &selected_entity_;
}

void HierarchyPanel::Draw() {
  if (!scene_) {
    return;
  }

  ImGui::Text("%s", scene_->GetName().c_str());

  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 30);
  if (ImGui::ButtonTransparent(ICON_FA_PLUS, 30, 0)) {
    scene_->CreateEntity();
    modify_info.SetModified();
  }

  std::vector<Entity> entities_to_remove{};

  for (auto [uuid, entity] : scene_->entity_map_) {
    const std::string& name = entity.GetName();

    int id = static_cast<int>((uint64_t)uuid);

    ImGui::PushID(id);

    if (ImGui::Selectable(std::format("{0} {1}", ICON_FA_CUBE, name).c_str(),
                          selected_entity_ == entity)) {
      SetSelectedEntity(entity);
    }

    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Delete")) {
        // Handle logic to remove the entity
        entities_to_remove.push_back(entity);
      }

      ImGui::EndPopup();
    }

    ImGui::PopID();
  }

  for (auto entity : entities_to_remove) {
    scene_->DestroyEntity(entity);
    modify_info.SetModified();
  }
}