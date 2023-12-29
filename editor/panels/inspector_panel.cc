// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/inspector_panel.h"

#include <inttypes.h>

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "scene/components.h"
#include "scene/scene_manager.h"
#include "scene/static_mesh.h"
#include "scripting/script_engine.h"

#include "utils/imgui_utils.h"
#include "utils/modify_info.h"

InspectorPanel::InspectorPanel(Ref<HierarchyPanel> hierarchy_panel)
    : Panel(true),
      hierarchy_panel_(hierarchy_panel),
      model_importer_(BIND_FUNC(OnModelMetaWrite), AssetType::kStaticMesh) {}

void InspectorPanel::Draw() {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (!selected_entity) {
    return;
  }

  model_importer_.Render();

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

    if (ImGui::Button("Material")) {
      selected_entity.AddComponent<Material>();
    }

    if (ImGui::Button("Model")) {
      model_importer_.SetActive(true);
    }

    if (ImGui::Button("Script")) {
      selected_entity.AddComponent<ScriptComponent>();
    }

    ImGui::EndPopup();
    add_component_dialog_opened_ = true;
  } else {
    // Close the window if clicked elsewhere
    add_component_dialog_opened_ = false;
  }
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity,
                          UIFunction ui_function) {
  const ImGuiTreeNodeFlags tree_node_flags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
      ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
      ImGuiTreeNodeFlags_FramePadding;
  if (entity.HasComponent<T>()) {
    auto& component = entity.GetComponent<T>();
    ImVec2 content_region_available = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});

    float line_height = ImGui::GetIO().FontDefault->FontSize +
                        ImGui::GetStyle().FramePadding.y * 2.0f;
    ImGui::Separator();
    bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tree_node_flags,
                                  "%s", name.c_str());
    ImGui::PopStyleVar();
    ImGui::SameLine(content_region_available.x - line_height * 0.5f);
    if (ImGui::Button("+", ImVec2{line_height, line_height})) {
      ImGui::OpenPopup("ComponentSettings");
    }

    bool remove_component = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove component"))
        remove_component = true;

      ImGui::EndPopup();
    }

    if (open) {
      ui_function(component);
      ImGui::TreePop();
    }

    if (remove_component)
      entity.RemoveComponent<T>();
  }
}

void InspectorPanel::RenderComponentProperties(Entity selected_entity) {
  ImGui::PushID((uint32_t)selected_entity);

  if (advanced_) {
    DrawComponent<IdComponent>(ICON_FA_ID_BADGE " Id Component",
                               selected_entity, [](IdComponent& id_comp) {
                                 ImGui::Text("ID:");
                                 ImGui::SameLine();
                                 ImGui::Text("%" PRIu64, (uint64_t)id_comp.id);
                               });
  }

  DrawComponent<TagComponent>(
      ICON_FA_TAG " Tag Component", selected_entity, [](TagComponent& tag_comp) {
        ImGui::Text("Tag:");
        ImGui::SameLine();
        // TODO only accept unique names
        if (ImGui::InputText("##tag_input", &tag_comp.tag)) {
          modify_info.SetModified();
        }
      });

  DrawComponent<Transform>(
      ICON_FA_ARROWS_ALT " Transform", selected_entity,
      [](Transform& transform) {
        ImGui::Columns(2, "Transform Settings Columns");

        ImGui::Text("Position:");
        ImGui::NextColumn();
        if (ImGui::DragFloat3("##transform_position",
                              glm::value_ptr(transform.position))) {
          modify_info.SetModified();
        }

        ImGui::NextColumn();

        ImGui::Text("Rotation:");
        ImGui::NextColumn();
        if (ImGui::DragFloat3("##transform_rotation",
                              glm::value_ptr(transform.rotation))) {
          modify_info.SetModified();
        }

        ImGui::NextColumn();

        ImGui::Text("Scale:");
        ImGui::NextColumn();
        if (ImGui::DragFloat3("##transform_scale",
                              glm::value_ptr(transform.scale))) {
          modify_info.SetModified();
        }

        ImGui::Columns();
      });

  DrawComponent<CameraComponent>(
      ICON_FA_CAMERA_RETRO " Camera", selected_entity,
      [](CameraComponent& camera_comp) {
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
        if (ImGui::Checkbox("##is_orthographic",
                            &camera_comp.is_orthographic)) {
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
      });

  DrawComponent<ModelComponent>(
      ICON_FA_CUBES " Model Component", selected_entity,
      [this](ModelComponent& model_comp) {
        ImGui::Columns(2, "Model Component Columns");

        ImGui::Text("Model Path:");
        ImGui::NextColumn();

        std::string path = model_comp.model->info.GetAssetPath();
        if (ImGui::InputText("##model_path_input", &path,
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
          auto asset = AssetLibrary::LoadFromPath<Model>(path);
          if (asset) {
            model_comp.model = asset;
            modify_info.SetModified();
          } else {
            model_importer_.SetActive(true);
          }
        }

        ImGui::NextColumn();

        ImGui::Columns();
      });

  DrawComponent<Material>(
      ICON_FA_PICTURE_O " Material Component", selected_entity,
      [this](Material& material) {
        ImGui::Columns(2, "Material Columns");

        ImGui::Text("Albedo:");
        ImGui::NextColumn();
        if (ImGui::ColorEdit3("##material_albedo",
                              glm::value_ptr(material.albedo))) {
          modify_info.SetModified();
        }

        ImGui::NextColumn();

        ImGui::Text("Metallic:");
        ImGui::NextColumn();
        if (ImGui::DragFloat("##material_metallic", &material.metallic)) {
          modify_info.SetModified();
        }

        ImGui::NextColumn();

        ImGui::Text("Roughness:");
        ImGui::NextColumn();
        if (ImGui::DragFloat("##material_roughness", &material.roughness)) {
          modify_info.SetModified();
        }

        ImGui::NextColumn();

        ImGui::Text("AO:");
        ImGui::NextColumn();
        if (ImGui::DragFloat("##material_ao", &material.ao, 0.05f)) {
          modify_info.SetModified();
        }

        ImGui::Columns();
      });

  DrawComponent<ScriptComponent>(
      "Script", selected_entity,
      [selected_entity,
       scene = SceneManager::GetActive()](auto& component) mutable {
        bool script_class_exists =
            ScriptEngine::EntityClassExists(component.class_name);

        static char buffer[64];
        strcpy_s(buffer, sizeof(buffer), component.class_name.c_str());

        // TODO
        // UI::ScopedStyleColor textColor(
        //     ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !script_class_exists);

        if (ImGui::InputText("Class", buffer, sizeof(buffer))) {
          component.class_name = buffer;
          return;
        }

        // Fields
        bool is_scene_running = scene->IsRunning();
        if (is_scene_running) {
          Ref<ScriptInstance> script_instance =
              ScriptEngine::GetEntityScriptInstance(selected_entity.GetUUID());
          if (script_instance) {
            const auto& fields = script_instance->GetScriptClass()->GetFields();
            for (const auto& [name, field] : fields) {
              if (field.type == ScriptFieldType::kFloat) {
                float data = script_instance->GetFieldValue<float>(name);
                if (ImGui::DragFloat(name.c_str(), &data)) {
                  script_instance->SetFieldValue(name, data);
                }
              }
              // TODO other types
            }
          }
        } else {
          if (script_class_exists) {
            Ref<ScriptClass> entity_class =
                ScriptEngine::GetEntityClass(component.class_name);
            const auto& fields = entity_class->GetFields();

            auto& entity_fields =
                ScriptEngine::GetScriptFieldMap(selected_entity);
            for (const auto& [name, field] : fields) {
              // Field has been set in editor
              if (entity_fields.find(name) != entity_fields.end()) {
                ScriptFieldInstance& script_field = entity_fields.at(name);

                // Display control to set it maybe
                if (field.type == ScriptFieldType::kFloat) {
                  float data = script_field.GetValue<float>();
                  if (ImGui::DragFloat(name.c_str(), &data)) {
                    script_field.SetValue(data);

                    modify_info.SetModified();
                  }
                }

              } else {
                // Display control to set it maybe
                if (field.type == ScriptFieldType::kFloat) {
                  float data = 0.0f;
                  if (ImGui::DragFloat(name.c_str(), &data)) {
                    ScriptFieldInstance& field_instance = entity_fields[name];
                    field_instance.field = field;
                    field_instance.SetValue(data);

                    modify_info.SetModified();
                  }
                }
              }
            }
          }
        }
      });

  ImGui::PopID();
}

void InspectorPanel::OnModelMetaWrite(const std::string& meta_path) {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (selected_entity.HasComponent<ModelComponent>()) {
    return;
  }

  auto& modal_component = selected_entity.AddComponent<ModelComponent>();
  modal_component.model = AssetLibrary::LoadFromMeta<Model>(meta_path);
}
