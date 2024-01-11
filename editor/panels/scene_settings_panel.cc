// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/scene_settings_panel.h"

#include <imgui.h>

#include "scene/scene_manager.h"

#include "utils/modify_info.h"

namespace eve {

SceneSettingsPanel::SceneSettingsPanel(Ref<EditorCamera> camera)
    : camera_(camera) {
  // TODO deserialize
}

void SceneSettingsPanel::Draw() {
  scene_ = SceneManager::GetActive();

  // TODO skybox

  ImGui::SeparatorText("Editor Camera");

  if (ImGui::DragFloat("FOV", &camera_->fov)) {
    modify_info.SetModified();
  }

  if (ImGui::DragFloat("Near Clip", &camera_->near_clip)) {
    modify_info.SetModified();
  }

  if (ImGui::DragFloat("Far Clip", &camera_->far_clip)) {
    modify_info.SetModified();
  }
}

}  // namespace eve
