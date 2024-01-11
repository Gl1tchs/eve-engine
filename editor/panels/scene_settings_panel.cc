// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/scene_settings_panel.h"

#include <imgui.h>

#include "scene/scene_manager.h"

#include "utils/modify_info.h"

namespace eve {

SceneSettingsPanel::SceneSettingsPanel(Ref<SceneRenderer> scene_renderer,
                                       Ref<EditorCamera> camera)
    : scene_renderer_(scene_renderer), camera_(camera) {
  // TODO deserialize
}

void SceneSettingsPanel::Draw() {
  scene_ = SceneManager::GetActive();

  {
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

  {
    ImGui::SeparatorText("Scene Renderer");

    auto& settings = scene_renderer_->GetSettings();

    if (ImGui::Checkbox("Draw Grid On Runtime", &settings.draw_grid)) {
      modify_info.SetModified();
    }

    if (ImGui::Checkbox("Render Physics Bounds On Runtime",
                        &settings.render_physics_bounds)) {
      modify_info.SetModified();
    }
  }

  // TODO skybox
}

}  // namespace eve
