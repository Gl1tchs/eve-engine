// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "scene/entity.h"
#include "scene/scene.h"

#include "panels/panel.h"

class HierarchyPanel : public Panel {
  IMPL_PANEL("Hierarchy")

 public:
  HierarchyPanel() = default;

  void SetScene(const Ref<Scene>& scene);

  void SetSelectedEntity(Entity entity);
  Entity GetSelectedEntity() const { return selected_entity_; }

 protected:
  void Draw() override;

 private:
  Ref<Scene> scene_;
  Entity selected_entity_;
};
