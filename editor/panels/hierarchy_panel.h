// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"

#include "ui/panel.h"

namespace eve {
class HierarchyPanel : public Panel {
  EVE_IMPL_PANEL("Hierarchy")

 public:
  HierarchyPanel();

  void SetSelectedEntity(Entity entity);
  Entity GetSelectedEntity() {
    if (selected_entity_ &&
        !SceneManager::GetActive()->Exists(selected_entity_)) {
      return kInvalidEntity;
    }

    return selected_entity_;
  }

 protected:
  void Draw() override;

 private:
  void DrawEntity(Entity& entity, bool is_child = false);

  void DrawEntityContextMenu(Entity& entity);

  void DrawEntityDragDropSource(Entity& entity);

  void DrawEntityDragDropTarget(const Entity& new_parent);

 private:
  Entity selected_entity_;
  std::vector<Entity> entities_to_remove_;

  friend class EditorLayer;
};
}  // namespace eve
