// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/viewport_panel.h"

#include <ImGuizmo.h>
#include <imgui.h>

#include "core/event/input.h"

#include "utils/modify_info.h"

namespace eve {

ViewportPanel::ViewportPanel(Ref<FrameBuffer>& frame_buffer,
                             Ref<HierarchyPanel> hierarchy_panel,
                             Ref<EditorCamera> editor_camera)
    : Panel(true, frame_buffer->GetSize()),
      frame_buffer_(frame_buffer),
      hierarchy_panel_(hierarchy_panel),
      editor_camera_(editor_camera) {
  PushStyle(ImGuiStyleVar_WindowPadding, glm::vec2{0, 0});
  SetFlags(ImGuiWindowFlags_NoCollapse);
}

void ViewportPanel::Draw() {
  // Draw frame buffer image
  {
    uint64_t texture_id = frame_buffer_->GetTexture()->GetTextureID();
    ImGui::Image(reinterpret_cast<void*>(texture_id),
                 ImVec2{GetSize().x, GetSize().y}, ImVec2{0, 1}, ImVec2{1, 0});
  }

  if (!should_draw_gizmos_) {
    return;
  }

  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (selected_entity && operation_ != -1) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    float window_width = (float)ImGui::GetWindowWidth();
    float window_height = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                      window_width, window_height);

    // Entity transform
    auto& tc = selected_entity.GetComponent<Transform>();

    // Snapping
    bool snap = Input::IsKeyPressed(KeyCode::kLeftControl);
    float snap_value = 0.5f;  // Snap to 0.5m for translation/scale
    // Snap to 45 degrees for rotation
    if (operation_ == ImGuizmo::OPERATION::ROTATE) {
      snap_value = 45.0f;
    }

    float snap_values[3] = {snap_value, snap_value, snap_value};

    // Editor camera
    const glm::mat4& camera_proj = editor_camera_->GetProjectionMatrix();
    glm::mat4 camera_view = editor_camera_->GetViewMatrix();

    float tmp_matrix[16];
    ImGuizmo::RecomposeMatrixFromComponents(
        glm::value_ptr(tc.position), glm::value_ptr(tc.rotation),
        glm::value_ptr(tc.scale), tmp_matrix);

    ImGuizmo::Manipulate(glm::value_ptr(camera_view),
                         glm::value_ptr(camera_proj),
                         (ImGuizmo::OPERATION)operation_, ImGuizmo::LOCAL,
                         tmp_matrix, nullptr, snap ? snap_values : nullptr);

    if (ImGuizmo::IsUsing()) {
      glm::vec3 position, rotation, scale;
      ImGuizmo::DecomposeMatrixToComponents(
          tmp_matrix, glm::value_ptr(position), glm::value_ptr(rotation),
          glm::value_ptr(scale));

      switch (operation_) {
        case ImGuizmo::OPERATION::TRANSLATE: {
          tc.position = position;
          break;
        }
        case ImGuizmo::OPERATION::ROTATE: {
          glm::vec3 delta_rotation = rotation - tc.rotation;
          tc.rotation += delta_rotation;
          break;
        }
        case ImGuizmo::OPERATION::SCALE: {
          tc.scale = scale;
          break;
        }
      }

      modify_info.SetModified();
    }
  }
}

}  // namespace eve
