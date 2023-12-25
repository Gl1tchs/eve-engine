// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/inspector_panel.h"

#include <inttypes.h>

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <cstdint>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

#include "scene/components.h"
#include "scripting/script_engine.h"

#include "utils/imgui_utils.h"
#include "utils/modify_info.h"

InspectorPanel::InspectorPanel(Ref<HierarchyPanel> hierarchy_panel)
    : Panel(true),
      hierarchy_panel_(hierarchy_panel),
      model_importer_(BIND_FUNC(OnModelMetaWrite), AssetType::kStaticMesh),
      script_importer_(BIND_FUNC(OnScriptPathWrite), AssetType::kScript) {}

void InspectorPanel::Draw() {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (!selected_entity) {
    return;
  }

  model_importer_.Render();
  script_importer_.Render();

  RenderComponentProperties(selected_entity);

  ImGui::Separator();

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

    if (ImGui::Button("Directional Light")) {
      selected_entity.AddComponent<DirectionalLight>();
    }

    if (ImGui::Button("Material")) {
      selected_entity.AddComponent<Material>();
    }

    if (ImGui::Button("Model")) {
      model_importer_.SetActive(true);
    }

    if (ImGui::Button("Script")) {
      script_importer_.SetActive(true);
    }

    ImGui::EndPopup();
    add_component_dialog_opened_ = true;
  } else {
    // Close the window if clicked elsewhere
    add_component_dialog_opened_ = false;
  }
}

#define COMPONENT_HEADER(name)                                \
  ImGui::Separator();                                         \
  bool should_remove = false;                                 \
  ImGui::Text(name);                                          \
  ImGui::PushID(name "_remove_button");                       \
  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 30); \
  if (ImGui::ButtonTransparent(ICON_FA_MINUS, 30, 0)) {       \
    should_remove = true;                                     \
  }                                                           \
  ImGui::PopID();

#define REMOVE_COMPONENT_IF_NEEDED(ComponentType)     \
  if (should_remove) {                                \
    selected_entity.RemoveComponent<ComponentType>(); \
    modify_info.SetModified();                        \
  }

void InspectorPanel::RenderComponentProperties(Entity selected_entity) {
  ImGui::PushID((uint32_t)selected_entity);

  // TODO if advanced
  if (advanced_) {
    ImGui::Separator();
    ImGui::Text(ICON_FA_ID_BADGE " Id Component");
    {
      auto& id_comp = selected_entity.GetComponent<IdComponent>();

      ImGui::Text("ID:");
      ImGui::SameLine();
      ImGui::Text("%" PRIu64, (uint64_t)id_comp.id);
    }
  }

  ImGui::Separator();
  ImGui::Text(ICON_FA_TAG " Tag Component");
  {
    auto& tag_comp = selected_entity.GetComponent<TagComponent>();

    ImGui::Text("Tag:");
    ImGui::SameLine();
    // TODO only accept unique names
    if (ImGui::InputText("##tag_input", &tag_comp.tag)) {
      modify_info.SetModified();
    }
  }

  ImGui::Separator();
  ImGui::Text(ICON_FA_ARROWS_ALT " Transform");
  {
    auto& transform = selected_entity.GetTransform();

    ImGui::Columns(2, "Transform Settings Columns");

    ImGui::Text("Position:");
    ImGui::NextColumn();
    if (ImGui::DragFloat3("##transform_position",
                          glm::value_ptr(transform.position), 0.05f)) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Rotation:");
    ImGui::NextColumn();
    if (ImGui::DragFloat3("##transform_rotation",
                          glm::value_ptr(transform.rotation), 0.05f)) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Scale:");
    ImGui::NextColumn();
    if (ImGui::DragFloat3("##transform_scale", glm::value_ptr(transform.scale),
                          0.05f)) {
      modify_info.SetModified();
    }

    ImGui::Columns();
  }

  if (selected_entity.HasComponent<CameraComponent>()) {
    COMPONENT_HEADER(ICON_FA_CAMERA_RETRO " Camera Component")

    auto& camera_comp = selected_entity.GetComponent<CameraComponent>();

    ImGui::Columns(2, "Camera Settings Columns");

    if (camera_comp.is_orthographic) {
      auto& camera = camera_comp.ortho_camera;

      ImGui::Text("Zoom Level:");
      ImGui::NextColumn();
      if (ImGui::DragFloat("##ortho_zoom_level", &camera.zoom_level)) {
        modify_info.SetModified();
      }

      ImGui::NextColumn();

      ImGui::Text("Near Clip:");
      ImGui::NextColumn();
      if (ImGui::DragFloat("##ortho_near_clip", &camera.near_clip)) {
        modify_info.SetModified();
      }

      ImGui::NextColumn();

      ImGui::Text("Far Clip:");
      ImGui::NextColumn();
      if (ImGui::DragFloat("##ortho_far_clip", &camera.far_clip)) {
        modify_info.SetModified();
      }
    } else {
      auto& camera = camera_comp.persp_camera;

      ImGui::Text("FOV:");
      ImGui::NextColumn();
      if (ImGui::DragFloat("##persp_fov", &camera.fov)) {
        modify_info.SetModified();
      }

      ImGui::NextColumn();

      ImGui::Text("Near Clip:");
      ImGui::NextColumn();
      if (ImGui::DragFloat("##persp_near_clip", &camera.near_clip)) {
        modify_info.SetModified();
      }

      ImGui::NextColumn();

      ImGui::Text("Far Clip:");
      ImGui::NextColumn();
      if (ImGui::DragFloat("##persp_far_clip", &camera.far_clip)) {
        modify_info.SetModified();
      }
    }

    ImGui::NextColumn();

    ImGui::Text("Is Orthographic");
    ImGui::NextColumn();
    if (ImGui::Checkbox("##is_orthographic", &camera_comp.is_orthographic)) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Primary:");
    ImGui::NextColumn();
    if (ImGui::Checkbox("##is_primary", &camera_comp.is_primary)) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Fixed");
    ImGui::NextColumn();
    if (ImGui::Checkbox("##fixed_aspect_ratio",
                        &camera_comp.is_fixed_aspect_ratio)) {
      modify_info.SetModified();
    }

    if (camera_comp.is_fixed_aspect_ratio) {
      ImGui::NextColumn();
      ImGui::Text("Aspect Ratio:");
      ImGui::NextColumn();
      float aspect_ratio;
      if (ImGui::DragFloat("##camera_aspect_ratio", &aspect_ratio, 0.05f)) {
        camera_comp.ortho_camera.aspect_ratio = aspect_ratio;
        camera_comp.persp_camera.aspect_ratio = aspect_ratio;
        modify_info.SetModified();
      }
    }

    ImGui::Columns();

    REMOVE_COMPONENT_IF_NEEDED(CameraComponent)
  }

  if (selected_entity.HasComponent<DirectionalLight>()) {
    COMPONENT_HEADER(ICON_FA_SUN_O " Directional Light");

    auto& light = selected_entity.GetComponent<DirectionalLight>();

    ImGui::Columns(2, "Directional Light Columns");

    ImGui::Text("Ambient:");
    ImGui::NextColumn();
    if (ImGui::ColorEdit3("##directional_light_ambient",
                          glm::value_ptr(light.ambient))) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Diffuse:");
    ImGui::NextColumn();
    if (ImGui::ColorEdit3("##directional_light_diffuse",
                          glm::value_ptr(light.diffuse))) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Specular:");
    ImGui::NextColumn();
    if (ImGui::ColorEdit3("##directional_light_specular",
                          glm::value_ptr(light.specular))) {
      modify_info.SetModified();
    }

    ImGui::Columns();

    REMOVE_COMPONENT_IF_NEEDED(DirectionalLight)
  }

  if (selected_entity.HasComponent<ModelComponent>()) {
    COMPONENT_HEADER(ICON_FA_CUBES " Model Component")

    auto& model_comp = selected_entity.GetComponent<ModelComponent>();

    ImGui::Columns(2, "Directional Light Columns");

    ImGui::Text("Model Path:");
    ImGui::NextColumn();

    std::string path = model_comp.model->info.GetAssetPath();
    if (ImGui::InputText("##model_path_input", &path,
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      model_comp.model = AssetLibrary::LoadFromPath<Model>(path);
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Columns();

    REMOVE_COMPONENT_IF_NEEDED(ModelComponent)
  }

  if (selected_entity.HasComponent<Material>()) {
    COMPONENT_HEADER(ICON_FA_PICTURE_O " Material Component")

    auto& material = selected_entity.GetComponent<Material>();

    ImGui::Columns(2, "Material Columns");

    ImGui::Text("Ambient:");
    ImGui::NextColumn();
    if (ImGui::ColorEdit3("##material_ambient",
                          glm::value_ptr(material.ambient))) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Diffuse:");
    ImGui::NextColumn();
    if (ImGui::ColorEdit3("##material_diffuse",
                          glm::value_ptr(material.diffuse))) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Specular:");
    ImGui::NextColumn();
    if (ImGui::ColorEdit3("##material_specular",
                          glm::value_ptr(material.specular))) {
      modify_info.SetModified();
    }

    ImGui::NextColumn();

    ImGui::Text("Shininess:");
    ImGui::NextColumn();
    if (ImGui::DragFloat("##material_shininess", &material.shininess, 0.05f)) {
      modify_info.SetModified();
    }

    ImGui::Columns();

    REMOVE_COMPONENT_IF_NEEDED(Material)
  }

  if (selected_entity.HasComponent<ScriptComponent>()) {
    COMPONENT_HEADER(ICON_FA_FILE_CODE_O " Script Component")

    auto& script_comp = selected_entity.GetComponent<ScriptComponent>();

    ImGui::Columns(2, "Script Field Columns");

    ImGui::Text("Script Path:");
    ImGui::NextColumn();
    if (ImGui::InputText("##script_path_input", &script_comp.path,
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      Ref<Script> script_instance =
          ScriptEngine::CreateScript(script_comp.path);
      // only update if instance exists
      if (script_instance) {
        script_comp.instance = script_instance;
        modify_info.SetModified();
      } else {
        script_comp.instance = nullptr;
        script_comp.path = "";
      }
    }

    ImGui::NextColumn();

    if (script_comp.instance) {
      auto& serialize_map = script_comp.instance->GetSerializeMap();

      for (auto& [name, data] : serialize_map) {

        // remove trailing "_"
        std::string var_name = name;
        if (var_name.ends_with("_")) {
          var_name.pop_back();
        }

        // put an "*" to show field overrided.
        ImGui::Text("%s %s", data.is_overrided ? "*" : "", var_name.c_str());
        ImGui::NextColumn();

        if (std::holds_alternative<int>(data.value)) {
          int int_value = std::get<int>(data.value);
          if (ImGui::InputInt(
                  std::format(
                      "##{}_field_{}",
                      std::filesystem::path(script_comp.path).stem().string(),
                      name)
                      .c_str(),
                  &int_value)) {
            modify_info.SetModified();
            // Update the value in the serialize_map
            script_comp.instance->SetSerializeDataField(name, int_value);
          }
        } else if (std::holds_alternative<float>(data.value)) {
          float float_value = std::get<float>(data.value);
          if (ImGui::InputFloat(
                  std::format(
                      "##{}_field_{}",
                      std::filesystem::path(script_comp.path).stem().string(),
                      name)
                      .c_str(),
                  &float_value)) {
            modify_info.SetModified();
            // Update the value in the serialize_map
            script_comp.instance->SetSerializeDataField(name, float_value);
          }
        } else if (std::holds_alternative<std::string>(data.value)) {
          std::string string_value = std::get<std::string>(data.value);
          if (ImGui::InputText(
                  std::format(
                      "##{}_field_{}",
                      std::filesystem::path(script_comp.path).stem().string(),
                      name)
                      .c_str(),
                  &string_value)) {
            modify_info.SetModified();
            // Update the value in the serialize_map
            script_comp.instance->SetSerializeDataField(name, string_value);
          }
        } else {
          // Handle additional types as needed
          // You may want to log an error or provide a default case
          LOG_ERROR("Unsupported variable type");
        }

        ImGui::NextColumn();
      }

      // skip two columns with this
      ImGui::NextColumn();

      if (ImGui::Button("Reload")) {
        script_comp.instance->Reload();
      }

      ImGui::NextColumn();
    }

    ImGui::Columns();

    REMOVE_COMPONENT_IF_NEEDED(ScriptComponent)
  }

  ImGui::PopID();
}

void InspectorPanel::OnModelMetaWrite(const std::string& meta_path) {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();

  auto& modal_component = selected_entity.AddComponent<ModelComponent>();
  modal_component.model = AssetLibrary::LoadFromMeta<Model>(meta_path);
}

void InspectorPanel::OnScriptPathWrite(const std::string& script_path) {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();

  auto& script_component = selected_entity.AddComponent<ScriptComponent>();
  script_component.path = script_path;
  script_component.instance = ScriptEngine::CreateScript(script_path);
}
