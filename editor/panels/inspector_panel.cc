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

namespace eve {
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

static void DrawScriptField(const std::string& name,
                            ScriptFieldInstance& script_field,
                            bool use_default = false);

static void DrawScriptFieldRuntime(const std::string& name,
                                   const ScriptField& field,
                                   Ref<ScriptInstance>& script_instance);

void InspectorPanel::RenderComponentProperties(Entity selected_entity) {
  ImGui::PushID((uint32_t)selected_entity);

  if (advanced_) {
    DrawComponent<IdComponent>(
        ICON_FA_ID_BADGE " Id Component", selected_entity,
        [](IdComponent& id_comp) {
          auto id_str = std::format("%" PRIu64, (uint64_t)id_comp.id);
          ImGui::InputText("ID", &id_str, ImGuiInputTextFlags_ReadOnly);
        });
  }

  DrawComponent<TagComponent>(ICON_FA_TAG " Tag Component", selected_entity,
                              [](TagComponent& tag_comp) {
                                // TODO only accept unique names
                                if (ImGui::InputText("Tag", &tag_comp.tag)) {
                                  modify_info.SetModified();
                                }
                              });

  DrawComponent<Transform>(
      ICON_FA_ARROWS_ALT " Transform", selected_entity,
      [](Transform& transform) {
        if (ImGui::DragFloat3("Position", glm::value_ptr(transform.position))) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation))) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale))) {
          modify_info.SetModified();
        }
      });

  DrawComponent<CameraComponent>(
      ICON_FA_CAMERA_RETRO " Camera", selected_entity,
      [](CameraComponent& camera_comp) {
        if (camera_comp.is_orthographic) {
          auto& camera = camera_comp.ortho_camera;

          if (ImGui::DragFloat("Zoom Level", &camera.zoom_level)) {
            modify_info.SetModified();
          }

          if (ImGui::DragFloat("Near Clip", &camera.near_clip)) {
            modify_info.SetModified();
          }

          if (ImGui::DragFloat("Far Clip", &camera.far_clip)) {
            modify_info.SetModified();
          }
        } else {
          auto& camera = camera_comp.persp_camera;

          if (ImGui::DragFloat("FOV", &camera.fov)) {
            modify_info.SetModified();
          }

          if (ImGui::DragFloat("Near Clip", &camera.near_clip)) {
            modify_info.SetModified();
          }

          if (ImGui::DragFloat("Far Clip", &camera.far_clip)) {
            modify_info.SetModified();
          }
        }

        if (ImGui::Checkbox("Is Orthographic", &camera_comp.is_orthographic)) {
          modify_info.SetModified();
        }

        if (ImGui::Checkbox("Is Primary", &camera_comp.is_primary)) {
          modify_info.SetModified();
        }

        if (ImGui::Checkbox("Is Fixed", &camera_comp.is_fixed_aspect_ratio)) {
          modify_info.SetModified();
        }

        if (camera_comp.is_fixed_aspect_ratio) {
          float aspect_ratio;
          if (ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.05f)) {
            camera_comp.ortho_camera.aspect_ratio = aspect_ratio;
            camera_comp.persp_camera.aspect_ratio = aspect_ratio;
            modify_info.SetModified();
          }
        }
      });

  DrawComponent<ModelComponent>(
      ICON_FA_CUBES " Model Component", selected_entity,
      [this](ModelComponent& model_comp) {
        std::string path = model_comp.model->info.GetAssetPath();
        if (ImGui::InputText("Path", &path,
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
          auto asset = AssetLibrary::LoadFromPath<Model>(path);
          if (asset) {
            model_comp.model = asset;
            modify_info.SetModified();
          } else {
            model_importer_.SetActive(true);
          }
        }
      });

  DrawComponent<Material>(
      ICON_FA_PICTURE_O " Material Component", selected_entity,
      [this](Material& material) {
        if (ImGui::ColorEdit3("Albedo", glm::value_ptr(material.albedo))) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat("Metallic", &material.metallic)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat("Roughness", &material.roughness)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat("AO", &material.ao, 0.05f)) {
          modify_info.SetModified();
        }
      });

  DrawComponent<ScriptComponent>(
      "Script", selected_entity,
      [selected_entity,
       scene = SceneManager::GetActive()](auto& component) mutable {
        bool script_class_exists =
            ScriptEngine::EntityClassExists(component.class_name);

        // if script class doesn't exists make the color red
        ImGui::ScopedStyleColor _color(ImGuiCol_Text,
                                       glm::vec4(0.9f, 0.2f, 0.3f, 1.0f),
                                       !script_class_exists);

        std::string class_name = component.class_name;
        if (ImGui::InputText("Class Name", &class_name)) {
          component.class_name = class_name;
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
              DrawScriptFieldRuntime(name, field, script_instance);
            }
          }
        } else if (!is_scene_running && script_class_exists) {
          Ref<ScriptClass> entity_class =
              ScriptEngine::GetEntityClass(component.class_name);
          const auto& fields = entity_class->GetFields();

          auto& entity_fields =
              ScriptEngine::GetScriptFieldMap(selected_entity);
          for (const auto& [name, field] : fields) {
            // Field has been set in editor
            if (entity_fields.find(name) != entity_fields.end()) {
              ScriptFieldInstance& script_field = entity_fields.at(name);
              DrawScriptField(name, script_field);
            } else {
              ScriptFieldInstance& script_field = entity_fields[name];
              script_field.field = field;

              DrawScriptField(name, script_field, true);
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

void DrawScriptField(const std::string& name, ScriptFieldInstance& script_field,
                     bool use_default) {
  switch (script_field.field.type) {
    case ScriptFieldType::kFloat: {
      float data = !use_default ? script_field.GetValue<float>() : 0.0f;
      if (ImGui::DragFloat(name.c_str(), &data)) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kDouble: {
      double data = !use_default ? script_field.GetValue<double>() : 0.0;
      if (ImGui::DragFloat(name.c_str(), (float*)&data)) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kBool: {
      bool data = !use_default ? script_field.GetValue<bool>() : false;
      if (ImGui::Checkbox(name.c_str(), &data)) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kChar: {
      break;
    }
    case ScriptFieldType::kByte: {
      int8_t data = !use_default ? script_field.GetValue<int8_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int8_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kShort: {
      int16_t data = !use_default ? script_field.GetValue<int16_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int16_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kInt: {
      int data = !use_default ? script_field.GetValue<int>() : 0;
      if (ImGui::DragInt(name.c_str(), &data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kLong: {
      int64_t data = !use_default ? script_field.GetValue<int64_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int64_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kUByte: {
      uint8_t data = !use_default ? script_field.GetValue<uint8_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint8_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kUShort: {
      uint16_t data = !use_default ? script_field.GetValue<uint16_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint16_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kUInt: {
      uint32_t data = !use_default ? script_field.GetValue<uint32_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint32_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kULong: {
      uint64_t data = !use_default ? script_field.GetValue<uint64_t>() : 0;
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint64_t>::max())) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kVector2: {
      glm::vec2 data =
          !use_default ? script_field.GetValue<glm::vec2>() : glm::vec2(0.0f);
      if (ImGui::DragFloat2(name.c_str(), &data[0])) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kVector3: {
      glm::vec3 data =
          !use_default ? script_field.GetValue<glm::vec3>() : glm::vec3(0.0f);
      if (ImGui::DragFloat3(name.c_str(), &data[0])) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kVector4: {
      glm::vec4 data =
          !use_default ? script_field.GetValue<glm::vec4>() : glm::vec4(0.0f);
      if (ImGui::DragFloat4(name.c_str(), &data[0])) {
        script_field.SetValue(data);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kScriptEntity: {
      break;
    }
  }
}

void DrawScriptFieldRuntime(const std::string& name, const ScriptField& field,
                            Ref<ScriptInstance>& script_instance) {
  switch (field.type) {
    case ScriptFieldType::kFloat: {
      float data = script_instance->GetFieldValue<float>(name);
      if (ImGui::DragFloat(name.c_str(), &data)) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kDouble: {
      double data = script_instance->GetFieldValue<double>(name);
      if (ImGui::DragFloat(name.c_str(), (float*)&data)) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kBool: {
      bool data = script_instance->GetFieldValue<bool>(name);
      if (ImGui::Checkbox(name.c_str(), &data)) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kChar: {
      break;
    }
    case ScriptFieldType::kByte: {
      int8_t data = script_instance->GetFieldValue<int8_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int8_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kShort: {
      int16_t data = script_instance->GetFieldValue<int16_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int16_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kInt: {
      int data = script_instance->GetFieldValue<int>(name);
      if (ImGui::DragInt(name.c_str(), &data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kLong: {
      int64_t data = script_instance->GetFieldValue<int64_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<int64_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kUByte: {
      uint8_t data = script_instance->GetFieldValue<uint8_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint8_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kUShort: {
      uint16_t data = script_instance->GetFieldValue<uint16_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint16_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kUInt: {
      uint32_t data = script_instance->GetFieldValue<uint32_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint32_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kULong: {
      uint64_t data = script_instance->GetFieldValue<uint64_t>(name);
      if (ImGui::DragInt(name.c_str(), (int*)&data, 1.0f,
                         std::numeric_limits<int8_t>::min(),
                         std::numeric_limits<uint64_t>::max())) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kVector2: {
      glm::vec2 data = script_instance->GetFieldValue<glm::vec2>(name);
      if (ImGui::DragFloat2(name.c_str(), &data[0])) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kVector3: {
      glm::vec3 data = script_instance->GetFieldValue<glm::vec3>(name);
      if (ImGui::DragFloat3(name.c_str(), &data[0])) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kVector4: {
      glm::vec4 data = script_instance->GetFieldValue<glm::vec4>(name);
      if (ImGui::DragFloat4(name.c_str(), &data[0])) {
        script_instance->SetFieldValue(name, data);
      }
      break;
    }
    case ScriptFieldType::kScriptEntity: {
      break;
    }
  }
}
}  // namespace eve
