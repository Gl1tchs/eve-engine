// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"

#include "panels/panel.h"

class HierarchyPanel : public Panel {
  IMPL_PANEL("Hierarchy")

 public:
  HierarchyPanel();

  void SetSelectedEntity(Entity entity);
  Entity GetSelectedEntity() {
    if (selected_entity_ &&
        !SceneManager::GetActive()->Exists(selected_entity_)) {
      return Entity{};
    }

    return selected_entity_;
  }

 protected:
  void Draw() override;

 private:
  Entity selected_entity_;
};
