// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/inspector_panel.h"

#include <inttypes.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

#include "scene/components.h"

InspectorPanel::InspectorPanel(Ref<HierarchyPanel> hierarchy_panel)
    : hierarchy_panel_(hierarchy_panel) {}

void InspectorPanel::Draw() {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (!selected_entity) {
    return;
  }

  ImGui::SeparatorText("Id Component");
  {
    auto id_comp = selected_entity.GetComponent<IdComponent>();

    ImGui::Text("ID:");
    ImGui::SameLine();
    ImGui::Text("%" PRIu64, (uint64_t)id_comp.id);
  }

  ImGui::SeparatorText("Tag Component");
  {
    auto& tag_comp = selected_entity.GetComponent<TagComponent>();

    ImGui::Text("Tag:");
    ImGui::SameLine();
    if (ImGui::InputText("##tag_input", &tag_comp.tag)) {
      modified_ = true;
    }
  }

  ImGui::SeparatorText("Transform");
  {
    auto& transform = selected_entity.GetTransform();

    ImGui::Text("Position:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##transform_position",
                          glm::value_ptr(transform.position), 0.05f)) {
      modified_ = true;
    }

    ImGui::Text("Rotation:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##transform_rotation",
                          glm::value_ptr(transform.rotation), 0.05f)) {
      modified_ = true;
    }

    ImGui::Text("Scale:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##transform_scale", glm::value_ptr(transform.scale),
                          0.05f)) {
      modified_ = true;
    }
  }

  if (selected_entity.HasComponent<CameraComponent>()) {
    ImGui::SeparatorText("Camera Component");

    auto& camera_comp = selected_entity.GetComponent<CameraComponent>();

    if (camera_comp.is_orthographic) {
      auto& camera = camera_comp.ortho_camera;

      ImGui::Text("Zoom Level:");
      float zoom_level = camera.GetZoomLevel();
      if (ImGui::DragFloat("##ortho_zoom_level", &zoom_level)) {
        modified_ = true;
      }
      camera.SetZoomLevel(zoom_level);

      ImGui::Text("Near Clip:");
      float near_clip = camera.GetNearClip();
      if (ImGui::DragFloat("##ortho_near_clip", &near_clip)) {
        modified_ = true;
      }
      camera.SetNearClip(near_clip);

      ImGui::Text("Far Clip:");
      float far_clip = camera.GetFarClip();
      if (ImGui::DragFloat("##ortho_far_clip", &far_clip)) {
        modified_ = true;
      }
      camera.SetFarClip(far_clip);

      if (camera_comp.is_fixed_aspect_ratio) {
        ImGui::Text("Aspect Ratio:");
        ImGui::SameLine();
        float aspect_ratio = camera.GetAspectRatio();
        if (ImGui::DragFloat("##ortho_aspect_ratio", &aspect_ratio, 0.05f)) {
          modified_ = true;
        }
        camera.SetAspectRatio(aspect_ratio);
      }
    } else {
      auto& camera = camera_comp.persp_camera;

      ImGui::Text("FOV:");
      float zoom_level = camera.GetFov();
      if (ImGui::DragFloat("##persp_zoom_level", &zoom_level)) {
        modified_ = true;
      }
      camera.SetFov(zoom_level);

      ImGui::Text("Near Clip:");
      float near_clip = camera.GetNearClip();
      if (ImGui::DragFloat("##persp_near_clip", &near_clip)) {
        modified_ = true;
      }
      camera.SetNearClip(near_clip);

      ImGui::Text("Far Clip:");
      float far_clip = camera.GetFarClip();
      if (ImGui::DragFloat("##persp_far_clip", &far_clip)) {
        modified_ = true;
      }
      camera.SetFarClip(far_clip);

      if (camera_comp.is_fixed_aspect_ratio) {
        ImGui::Text("Aspect Ratio:");
        ImGui::SameLine();
        float aspect_ratio = camera.GetAspectRatio();
        if (ImGui::DragFloat("##persp_aspect_ratio", &aspect_ratio, 0.05f)) {
          modified_ = true;
        }
        camera.SetAspectRatio(aspect_ratio);
      }
    }

    ImGui::Text("Primary:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##is_primary", &camera_comp.is_primary)) {
      modified_ = true;
    }

    ImGui::Text("Fixed");
    ImGui::SameLine();
    if (ImGui::Checkbox("##fixed_aspect_ratio",
                        &camera_comp.is_fixed_aspect_ratio)) {
      modified_ = true;
    }
  }
}
