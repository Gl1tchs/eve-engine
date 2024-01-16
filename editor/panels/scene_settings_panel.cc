// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/scene_settings_panel.h"

#include <IconsFontAwesome4.h>
#include <imgui.h>

#include "scene/scene_manager.h"
#include "ui/imgui_utils.h"

#include "utils/modify_info.h"

namespace eve {

SceneSettingsPanel::SceneSettingsPanel(Ref<SceneRenderer> scene_renderer,
                                       Ref<EditorCamera> camera)
    : Panel(true), scene_renderer_(scene_renderer), camera_(camera) {
  // TODO deserialize
}

void SceneSettingsPanel::Draw() {
  scene_ = SceneManager::GetActive();

  ImGui::DrawTreeNode(ICON_FA_VIDEO_CAMERA " Editor Camera", [this]() {
    if (ImGui::DragFloat("FOV", &camera_->fov)) {
      modify_info.SetModified();
    }

    if (ImGui::DragFloat("Near Clip", &camera_->near_clip)) {
      modify_info.SetModified();
    }

    if (ImGui::DragFloat("Far Clip", &camera_->far_clip)) {
      modify_info.SetModified();
    }

    float sensitivity = camera_->GetSensitivity();
    if (ImGui::DragFloat("Sensitivity", &sensitivity)) {
      camera_->SetSensitivity(sensitivity);
      modify_info.SetModified();
    }

    float speed = camera_->GetSpeed();
    if (ImGui::DragFloat("Speed", &speed)) {
      camera_->SetSpeed(speed);
      modify_info.SetModified();
    }
  });

  ImGui::DrawTreeNode("Scene Renderer", [this]() {
    auto& settings = scene_renderer_->GetSettings();

    if (ImGui::Checkbox("Draw Grid On Runtime", &settings.draw_grid)) {
      modify_info.SetModified();
    }

    if (ImGui::Checkbox("Render Physics Bounds On Runtime",
                        &settings.render_physics_bounds)) {
      modify_info.SetModified();
    }
  });

  // TODO skybox
}

}  // namespace eve
