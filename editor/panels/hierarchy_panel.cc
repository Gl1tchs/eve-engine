// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/hierarchy_panel.h"

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "scene/scene.h"
#include "utils/imgui_utils.h"
#include "utils/modify_info.h"

namespace eve {
HierarchyPanel::HierarchyPanel() : Panel(true), selected_entity_({}) {}

void HierarchyPanel::SetSelectedEntity(Entity entity) {
  selected_entity_ = entity;
  SceneManager::GetActive()->selected_entity_ = &selected_entity_;
}

void HierarchyPanel::Draw() {
  auto& scene = SceneManager::GetActive();
  if (!scene) {
    selected_entity_ = Entity{};
    return;
  }

  // If scene changed during runtime
  if (selected_entity_.scene_ != scene.get()) {
    selected_entity_ = Entity{selected_entity_.entity_handle_, scene.get()};
  }

  ImGui::Text("%s", scene->GetName().c_str());

  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 30);
  if (ImGui::ButtonTransparent(ICON_FA_PLUS, 30, 0)) {
    scene->CreateEntity();
    modify_info.SetModified();
  }

  std::vector<Entity> entities_to_remove{};

  for (auto [uuid, entity] : scene->entity_map_) {
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

    if (ImGui::BeginDragDropSource()) {
      ImGui::SetDragDropPayload("DND_PAYLOAD_ENTITY", &entity.GetUUID(),
                                sizeof(UUID));
      ImGui::EndDragDropSource();
    }

    ImGui::PopID();
  }

  for (auto entity : entities_to_remove) {
    scene->DestroyEntity(entity);
    modify_info.SetModified();
  }
}
}  // namespace eve
