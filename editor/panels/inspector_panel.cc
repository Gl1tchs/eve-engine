// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/inspector_panel.h"

#include <inttypes.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

#include "core/utils/functional.h"
#include "inspector_panel.h"
#include "scene/components.h"
#include "utils/modify_info.h"

InspectorPanel::InspectorPanel(Ref<HierarchyPanel> hierarchy_panel)
    : hierarchy_panel_(hierarchy_panel),
      model_importer_(BIND_FUNC(OnModelMetaWrite), AssetType::kStaticMesh) {}

void InspectorPanel::Draw() {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (!selected_entity) {
    return;
  }

  model_importer_.Render();

  RenderComponentProperties(selected_entity);

  ImGui::Spacing();
  ImGui::Separator();

  ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                       ImGui::GetContentRegionAvail().y -
                       ImGui::GetTextLineHeightWithSpacing());

  RenderAddComponentDialog(selected_entity);
}

void InspectorPanel::RenderAddComponentDialog(Entity selected_entity) {
  ImGui::Button("Add Component", ImVec2(-1, 0));  // Full width button
  if (ImGui::IsItemClicked()) {
    // Get the mouse position
    ImVec2 mouse_pos = ImGui::GetMousePos();

    // Open the popup under the cursor
    ImGui::OpenPopup("AddComponentPopup");
    ImGui::SetNextWindowPos(mouse_pos);

    // Reset the flag
    show_add_component_dialog_ = false;
  }

  // Check if the dialog is open and render it
  if (ImGui::BeginPopup(
          "AddComponentPopup",
          ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
              ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking |
              ImGuiWindowFlags_NoMove)) {
    ImGui::Text("Choose a component to add:");

    if (ImGui::Button("Camera")) {
      selected_entity.AddComponent<CameraComponent>();
    }

    if (ImGui::Button("Material")) {
      selected_entity.AddComponent<Material>();
    }

    if (ImGui::Button("Model")) {
      model_importer_.SetActive(true);
    }

    ImGui::EndPopup();
    add_component_dialog_opened_ = true;
  } else {
    // Close the window if clicked elsewhere
    add_component_dialog_opened_ = false;
  }
}

void InspectorPanel::RenderComponentProperties(Entity selected_entity) {
  // TODO if advanced
  ImGui::SeparatorText("Id Component");
  {
    auto& id_comp = selected_entity.GetComponent<IdComponent>();

    ImGui::Text("ID:");
    ImGui::SameLine();
    ImGui::Text("%" PRIu64, (uint64_t)id_comp.id);
  }

  ImGui::SeparatorText("Tag Component");
  {
    auto& tag_comp = selected_entity.GetComponent<TagComponent>();

    ImGui::Text("Tag:");
    ImGui::SameLine();
    // TODO only accept unique names
    if (ImGui::InputText("##tag_input", &tag_comp.tag)) {
      modify_info.SetModified();
    }
  }

  ImGui::SeparatorText("Transform");
  {
    auto& transform = selected_entity.GetTransform();

    ImGui::Text("Position:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##transform_position",
                          glm::value_ptr(transform.position), 0.05f)) {
      modify_info.SetModified();
    }

    ImGui::Text("Rotation:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##transform_rotation",
                          glm::value_ptr(transform.rotation), 0.05f)) {
      modify_info.SetModified();
    }

    ImGui::Text("Scale:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##transform_scale", glm::value_ptr(transform.scale),
                          0.05f)) {
      modify_info.SetModified();
    }
  }

  if (selected_entity.HasComponent<CameraComponent>()) {
    bool should_remove = false;

    ImGui::Separator();
    ImGui::Text("Camera Component");
    ImGui::SameLine(ImGui::GetWindowWidth() - 50);
    ImGui::PushID("camera_remove_button");
    if (ImGui::Button("-")) {
      should_remove = true;
    }
    ImGui::PopID();

    auto& camera_comp = selected_entity.GetComponent<CameraComponent>();

    if (camera_comp.is_orthographic) {
      auto& camera = camera_comp.ortho_camera;

      ImGui::Text("Zoom Level:");
      float zoom_level = camera.GetZoomLevel();
      if (ImGui::DragFloat("##ortho_zoom_level", &zoom_level)) {
        modify_info.SetModified();
      }
      camera.SetZoomLevel(zoom_level);

      ImGui::Text("Near Clip:");
      float near_clip = camera.GetNearClip();
      if (ImGui::DragFloat("##ortho_near_clip", &near_clip)) {
        modify_info.SetModified();
      }
      camera.SetNearClip(near_clip);

      ImGui::Text("Far Clip:");
      float far_clip = camera.GetFarClip();
      if (ImGui::DragFloat("##ortho_far_clip", &far_clip)) {
        modify_info.SetModified();
      }
      camera.SetFarClip(far_clip);

      if (camera_comp.is_fixed_aspect_ratio) {
        ImGui::Text("Aspect Ratio:");
        ImGui::SameLine();
        float aspect_ratio = camera.GetAspectRatio();
        if (ImGui::DragFloat("##ortho_aspect_ratio", &aspect_ratio, 0.05f)) {
          modify_info.SetModified();
        }
        camera.SetAspectRatio(aspect_ratio);
      }
    } else {
      auto& camera = camera_comp.persp_camera;

      ImGui::Text("FOV:");
      float zoom_level = camera.GetFov();
      if (ImGui::DragFloat("##persp_zoom_level", &zoom_level)) {
        modify_info.SetModified();
      }
      camera.SetFov(zoom_level);

      ImGui::Text("Near Clip:");
      float near_clip = camera.GetNearClip();
      if (ImGui::DragFloat("##persp_near_clip", &near_clip)) {
        modify_info.SetModified();
      }
      camera.SetNearClip(near_clip);

      ImGui::Text("Far Clip:");
      float far_clip = camera.GetFarClip();
      if (ImGui::DragFloat("##persp_far_clip", &far_clip)) {
        modify_info.SetModified();
      }
      camera.SetFarClip(far_clip);

      if (camera_comp.is_fixed_aspect_ratio) {
        ImGui::Text("Aspect Ratio:");
        ImGui::SameLine();
        float aspect_ratio = camera.GetAspectRatio();
        if (ImGui::DragFloat("##persp_aspect_ratio", &aspect_ratio, 0.05f)) {
          modify_info.SetModified();
        }
        camera.SetAspectRatio(aspect_ratio);
      }
    }

    ImGui::Text("Primary:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##is_primary", &camera_comp.is_primary)) {
      modify_info.SetModified();
    }

    ImGui::Text("Fixed");
    ImGui::SameLine();
    if (ImGui::Checkbox("##fixed_aspect_ratio",
                        &camera_comp.is_fixed_aspect_ratio)) {
      modify_info.SetModified();
    }

    if (should_remove) {
      selected_entity.RemoveComponent<CameraComponent>();
      modify_info.SetModified();
    }
  }

  if (selected_entity.HasComponent<DirectionalLight>()) {
    bool should_remove = false;

    ImGui::Separator();
    ImGui::Text("Directional Light");
    ImGui::SameLine(ImGui::GetWindowWidth() - 50);
    ImGui::PushID("directional_light_remove_button");
    if (ImGui::Button("-")) {
      should_remove = true;
    }
    ImGui::PopID();

    auto& light = selected_entity.GetComponent<DirectionalLight>();

    ImGui::Text("Ambient:");
    ImGui::SameLine();
    if (ImGui::ColorEdit3("##directional_light_ambient",
                          glm::value_ptr(light.ambient))) {
      modify_info.SetModified();
    }

    ImGui::Text("Diffuse:");
    ImGui::SameLine();
    if (ImGui::ColorEdit3("##directional_light_diffuse",
                          glm::value_ptr(light.diffuse))) {
      modify_info.SetModified();
    }

    ImGui::Text("Specular:");
    ImGui::SameLine();
    if (ImGui::ColorEdit3("##directional_light_specular",
                          glm::value_ptr(light.specular))) {
      modify_info.SetModified();
    }

    if (should_remove) {
      selected_entity.RemoveComponent<DirectionalLight>();
      modify_info.SetModified();
    }
  }

  if (selected_entity.HasComponent<ModelComponent>()) {
    bool should_remove = false;

    ImGui::Separator();
    ImGui::Text("Model");
    ImGui::SameLine(ImGui::GetWindowWidth() - 50);
    ImGui::PushID("model_remove_button");
    if (ImGui::Button("-")) {
      should_remove = true;
    }
    ImGui::PopID();

    auto& model_comp = selected_entity.GetComponent<ModelComponent>();

    ImGui::Text("Model Path:");
    ImGui::SameLine();

    std::string path = model_comp.model->info.GetAssetPath();
    if (ImGui::InputText("##model_path_input", &path,
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      model_comp.model = AssetLibrary::LoadFromPath<Model>(path);
      modify_info.SetModified();
    }

    if (should_remove) {
      selected_entity.RemoveComponent<ModelComponent>();
      modify_info.SetModified();
    }
  }

  if (selected_entity.HasComponent<Material>()) {
    bool should_remove = false;

    ImGui::Separator();
    ImGui::Text("Material");
    ImGui::SameLine(ImGui::GetWindowWidth() - 50);
    ImGui::PushID("material_remove_button");
    if (ImGui::Button("-")) {
      should_remove = true;
    }
    ImGui::PopID();

    auto& material = selected_entity.GetComponent<Material>();

    ImGui::Text("Ambient:");
    ImGui::SameLine();
    if (ImGui::ColorEdit3("##material_ambient",
                          glm::value_ptr(material.ambient))) {
      modify_info.SetModified();
    }

    ImGui::Text("Diffuse:");
    ImGui::SameLine();
    if (ImGui::ColorEdit3("##material_diffuse",
                          glm::value_ptr(material.diffuse))) {
      modify_info.SetModified();
    }

    ImGui::Text("Specular:");
    ImGui::SameLine();
    if (ImGui::ColorEdit3("##material_specular",
                          glm::value_ptr(material.specular))) {
      modify_info.SetModified();
    }

    ImGui::Text("Shininess:");
    ImGui::SameLine();
    if (ImGui::DragFloat("##material_shininess", &material.shininess, 0.05f)) {
      modify_info.SetModified();
    }

    if (should_remove) {
      selected_entity.RemoveComponent<Material>();
      modify_info.SetModified();
    }
  }
}

void InspectorPanel::OnModelMetaWrite(const std::string& meta_path) {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();

  auto& modal_component = selected_entity.AddComponent<ModelComponent>();
  modal_component.model = AssetLibrary::LoadFromMeta<Model>(meta_path);
}
