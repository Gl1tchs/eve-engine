// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/hierarchy_panel.h"

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "scene/scene.h"
#include "ui/imgui_utils.h"
#include "utils/modify_info.h"

namespace eve {

HierarchyPanel::HierarchyPanel() : Panel(true), selected_entity_({}) {}

void HierarchyPanel::SetSelectedEntity(Entity entity) {
  selected_entity_ = entity;
  if (auto& active_scene = SceneManager::GetActive(); active_scene) {
    active_scene->selected_entity_ = &selected_entity_;
  }
}

Entity HierarchyPanel::GetSelectedEntity() {
  if (selected_entity_ &&
      !SceneManager::GetActive()->Exists(selected_entity_)) {
    return kInvalidEntity;
  }

  return selected_entity_;
}

void HierarchyPanel::Draw() {
  auto& scene = SceneManager::GetActive();
  if (!scene) {
    selected_entity_ = kInvalidEntity;
    return;
  }

  // If scene changed during runtime
  if (selected_entity_.scene_ != scene.get()) {
    selected_entity_ = Entity{selected_entity_.entity_handle_, scene.get()};
  }

  ImGui::TextUnformatted(scene->GetName().c_str());

  ImGui::SameLine(ImGui::GetContentRegionMax().x -
                  (ImGui::CalcTextSize(ICON_FA_PLUS).x +
                   2 * ImGui::GetStyle().FramePadding.x));
  if (ImGui::ButtonTransparent(ICON_FA_PLUS)) {
    // create entity and set selected to new created entity
    selected_entity_ = scene->CreateEntity();
    modify_info.SetModified();
  }

  ImGui::Separator();

  entities_to_remove_.clear();

  for (auto [uuid, entity] : scene->GetAllEntities()) {
    ImGui::PushID((uint64_t)uuid);

    // If entity has not any parent (top level).
    if (!entity.GetParent()) {
      DrawEntity(entity);
    }

    ImGui::PopID();
  }

  ImGui::Dummy(
      ImVec2(ImGui::GetWindowWidth(), ImGui::GetContentRegionAvail().y));

  // If an item dragged here it will set as top object.
  DrawEntityDragDropTarget(kInvalidEntity);

  if (ImGui::IsItemClicked()) {
    selected_entity_ = kInvalidEntity;
  }

  for (auto entity : entities_to_remove_) {
    scene->DestroyEntity(entity);
    modify_info.SetModified();
  }
}

void HierarchyPanel::DrawEntity(Entity& entity, bool is_child) {
  ImGui::PushID((uint64_t)entity.GetUUID());

  const std::string hierarchy_name =
      std::format("{0} {1}", ICON_FA_CUBE, entity.GetName());

  const bool is_selected = selected_entity_ == entity;

  static const float entity_indent = 0.75f;

  if (!entity.IsParent()) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{1, 2});

    ImGui::Bullet();

    if (ImGui::Selectable(hierarchy_name.c_str(), is_selected)) {
      SetSelectedEntity(entity);
    }

    ImGui::PopStyleVar();

    // Drag and drop source and target to change relations.
    DrawEntityDragDropSource(entity);
    DrawEntityDragDropTarget(entity);
  } else {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{1, 2});

    bool node_open = ImGui::TreeNodeEx(
        hierarchy_name.c_str(),
        ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow |
            (is_selected ? ImGuiTreeNodeFlags_Selected : 0));

    ImGui::PopStyleVar();

    // Drag and drop source and target to change relations.
    DrawEntityDragDropSource(entity);
    DrawEntityDragDropTarget(entity);

    if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left) &&
        ImGui::IsItemClicked()) {
      SetSelectedEntity(entity);
    }

    if (node_open) {
      ImGui::Indent(entity_indent);

      for (Entity& child : entity.GetChildren()) {
        DrawEntity(child, true);
      }

      ImGui::Unindent(entity_indent);

      ImGui::TreePop();
    }
  }

  DrawEntityContextMenu(entity);

  ImGui::PopID();
}

void HierarchyPanel::DrawEntityContextMenu(Entity& entity) {
  auto& scene = SceneManager::GetActive();
  if (!scene || !entity) {
    return;
  }

  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Add")) {
      scene->CreateEntity();
      modify_info.SetModified();
    }

    if (ImGui::MenuItem("Add Child")) {
      scene->CreateEntity({"", entity.GetUUID()});
      modify_info.SetModified();
    }

    if (ImGui::MenuItem("Delete")) {
      // Handle logic to remove the entity
      entities_to_remove_.push_back(entity);
      modify_info.SetModified();
    }

    ImGui::EndPopup();
  }
}

void HierarchyPanel::DrawEntityDragDropSource(Entity& entity) {
  if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("DND_PAYLOAD_ENTITY", &entity.GetUUID(),
                              sizeof(UUID));

    ImGui::SetTooltip("%s %s", ICON_FA_CUBE, entity.GetName().c_str());

    ImGui::EndDragDropSource();
  }
}

void HierarchyPanel::DrawEntityDragDropTarget(const Entity& new_parent) {
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("DND_PAYLOAD_ENTITY")) {
      UUID recv_id = *static_cast<UUID*>(payload->Data);
      if (Entity recv_entity =
              SceneManager::GetActive()->TryGetEntityByUUID(recv_id);
          recv_entity) {
        recv_entity.SetParent(new_parent);
        modify_info.SetModified();
      }
    }

    ImGui::EndDragDropTarget();
  }
}

}  // namespace eve
