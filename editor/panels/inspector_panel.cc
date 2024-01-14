// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/inspector_panel.h"

#include <inttypes.h>

#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "asset/asset_registry.h"
#include "core/color.h"
#include "physics/box_collider.h"
#include "physics/rigidbody.h"
#include "scene/components.h"
#include "scene/model.h"
#include "scene/scene_manager.h"
#include "scripting/script_engine.h"
#include "ui/imgui_utils.h"

#include "utils/modify_info.h"

namespace eve {
InspectorPanel::InspectorPanel(Ref<HierarchyPanel> hierarchy_panel)
    : Panel(true), hierarchy_panel_(hierarchy_panel) {}

void InspectorPanel::Draw() {
  Entity selected_entity = hierarchy_panel_->GetSelectedEntity();
  if (!selected_entity) {
    return;
  }

  RenderEntityHeader(selected_entity);
  RenderComponentProperties(selected_entity);
}

template <typename T>
void DisplayAddComponentEntry(Entity& selected_entity,
                              const std::string& component_name) {
  if (!selected_entity.HasComponent<T>()) {
    if (ImGui::MenuItem(component_name.c_str())) {
      selected_entity.AddComponent<T>();
      ImGui::CloseCurrentPopup();

      modify_info.SetModified();
    }
  }
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity,
                          UIFunction ui_function) {
  if (!entity.HasComponent<T>()) {
    return;
  }

  constexpr ImGuiTreeNodeFlags kTreeNodeFlags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
      ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
      ImGuiTreeNodeFlags_FramePadding;

  auto& component = entity.GetComponent<T>();
  ImVec2 content_region_available = ImGui::GetContentRegionAvail();

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});

  const auto kFramePadding = ImGui::GetStyle().FramePadding;

  const float kLineHeight =
      ImGui::GetIO().FontDefault->FontSize + kFramePadding.y * 2.0f;

  ImGui::Separator();
  bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), kTreeNodeFlags,
                                "%s", name.c_str());
  ImGui::PopStyleVar();
  ImGui::SameLine(
      content_region_available.x -
      (ImGui::CalcTextSize(ICON_FA_ELLIPSIS_H).x + 2 * kFramePadding.x));
  if (ImGui::ButtonTransparent(ICON_FA_ELLIPSIS_H, kLineHeight, kLineHeight)) {
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

  if (remove_component) {
    entity.RemoveComponent<T>();
  }
}

void InspectorPanel::RenderEntityHeader(Entity selected_entity) {
  if (advanced_) {
    DrawComponent<IdComponent>(
        ICON_FA_ID_BADGE " Id Component", selected_entity,
        [](IdComponent& id_comp) {
          auto id_str = std::format("%" PRIu64, (uint64_t)id_comp.id);
          ImGui::InputText("ID", &id_str, ImGuiInputTextFlags_ReadOnly);
        });
  }

  const float kPlusButtonWidth = ImGui::CalcTextSize(ICON_FA_PLUS).x +
                                 2 * ImGui::GetStyle().FramePadding.x;

  if (selected_entity.HasComponent<TagComponent>()) {
    auto& tag_comp = selected_entity.GetComponent<TagComponent>();

    ImGui::PushItemWidth(ImGui::GetContentRegionMax().x - kPlusButtonWidth);

    ImGui::InputText("##tag", &tag_comp.tag);
  }

  ImGui::SameLine(ImGui::GetContentRegionMax().x - kPlusButtonWidth);

  if (ImGui::Button(ICON_FA_PLUS))
    ImGui::OpenPopup("AddComponent");

  if (ImGui::BeginPopup("AddComponent")) {
    DisplayAddComponentEntry<CameraComponent>(selected_entity, "Camera");
    DisplayAddComponentEntry<ScriptComponent>(selected_entity, "Script");
    DisplayAddComponentEntry<SpriteRendererComponent>(selected_entity,
                                                      "Sprite Renderer");
    DisplayAddComponentEntry<ModelComponent>(selected_entity, "Model");
    DisplayAddComponentEntry<Material>(selected_entity, "Material");
    DisplayAddComponentEntry<Rigidbody>(selected_entity, "Rigidbody");
    DisplayAddComponentEntry<BoxCollider>(selected_entity, "BoxCollider");

    ImGui::EndPopup();
  }
}

static void DrawUniformField(ShaderUniform& uniform);

static void DrawScriptField(const std::string& name,
                            ScriptFieldInstance& script_field,
                            bool use_default = false);

static void DrawScriptFieldRuntime(const std::string& name,
                                   const ScriptField& field,
                                   Ref<ScriptInstance>& script_instance);

void InspectorPanel::RenderComponentProperties(Entity selected_entity) {
  ImGui::PushID((uint32_t)selected_entity);

  DrawComponent<Transform>(
      ICON_FA_ARROWS_ALT " Transform", selected_entity,
      [](Transform& transform) {
        if (ImGui::DragFloat3("Position", glm::value_ptr(transform.position),
                              0.1f)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation),
                              0.1f)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f)) {
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

  DrawComponent<SpriteRendererComponent>(
      ICON_FA_PICTURE_O " Sprite Renderer Component", selected_entity,
      [this](SpriteRendererComponent& sprite_comp) {
        Ref<Texture> texture =
            sprite_comp.texture != 0
                ? AssetRegistry::Get<Texture>(sprite_comp.texture)
                : nullptr;

        if (!texture) {
          ImGui::TextUnformatted("Drag Texture Here");
        } else {
          ImGui::InputText("Texture", &texture->name,
                           ImGuiInputTextFlags_ReadOnly);
        }

        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload* payload =
                  ImGui::AcceptDragDropPayload("DND_PAYLOAD_TEXTURE")) {
            const AssetHandle handle = *(const AssetHandle*)payload->Data;
            if (AssetRegistry::Exists(handle)) {
              sprite_comp.texture = handle;
              modify_info.SetModified();
            }
          }
          ImGui::EndDragDropTarget();
        }

        if (ImGui::ColorEdit4("Color", &sprite_comp.color.r)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat2("Tiling", &sprite_comp.tex_tiling.x)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat2("Offset", &sprite_comp.tex_offset.x)) {
          modify_info.SetModified();
        }
      });

  DrawComponent<ModelComponent>(
      ICON_FA_CUBES " Model Component", selected_entity,
      [this](ModelComponent& model_comp) {
        Ref<Model> model = model_comp.model != 0
                               ? AssetRegistry::Get<Model>(model_comp.model)
                               : nullptr;

        if (!model) {
          ImGui::TextUnformatted("Drag Model Here");
        } else {
          ImGui::InputText("Model", &model->name, ImGuiInputTextFlags_ReadOnly);
        }

        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload* payload =
                  ImGui::AcceptDragDropPayload("DND_PAYLOAD_MESH")) {
            const AssetHandle handle = *(const AssetHandle*)payload->Data;
            if (AssetRegistry::Exists(handle)) {
              model_comp.model = handle;
              modify_info.SetModified();
            }
          }
          ImGui::EndDragDropTarget();
        }
      });

  DrawComponent<Material>(
      ICON_FA_PICTURE_O " Material Component", selected_entity,
      [this](Material& material) {
        if (ImGui::ColorEdit3("Albedo", &material.albedo.r)) {
          modify_info.SetModified();
        }

        Ref<ShaderInstance> shader_instance =
            material.shader != 0
                ? AssetRegistry::Get<ShaderInstance>(material.shader)
                : nullptr;

        if (!shader_instance) {
          ImGui::TextUnformatted("Drag Shader Here");
        } else {
          ImGui::InputText("Shader", &shader_instance->name,
                           ImGuiInputTextFlags_ReadOnly);
        }

        if (ImGui::BeginDragDropSource(
                ImGuiDragDropFlags_SourceNoDisableHover ||
                ImGuiDragDropFlags_SourceNoPreviewTooltip)) {
          ImGui::SetDragDropPayload("DND_PAYLOAD_SHADER",
                                    &shader_instance->handle,
                                    sizeof(AssetHandle));
          ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload* payload =
                  ImGui::AcceptDragDropPayload("DND_PAYLOAD_SHADER")) {
            const AssetHandle handle = *(const AssetHandle*)payload->Data;
            if (AssetRegistry::Exists(handle)) {
              material.shader = handle;
              modify_info.SetModified();
            }
          }
          ImGui::EndDragDropTarget();
        }

        if (shader_instance) {
          for (auto& uniform : shader_instance->uniforms) {
            DrawUniformField(uniform);
          }
        }
      });

  DrawComponent<Rigidbody>("Rigidbody", selected_entity, [](Rigidbody& rb) {
    if (ImGui::DragFloat3("Velocity", glm::value_ptr(rb.velocity))) {
      modify_info.SetModified();
    }

    if (ImGui::DragFloat3("Accelerationm", glm::value_ptr(rb.acceleration))) {
      modify_info.SetModified();
    }

    if (ImGui::DragFloat("Mass", &rb.mass)) {
      modify_info.SetModified();
    }

    if (ImGui::Checkbox("Use Gravity", &rb.use_gravity)) {
      modify_info.SetModified();
    }

    if (ImGui::TreeNode("Position Constraints")) {

      if (ImGui::Checkbox("Freeze X", &rb.position_constraints.freeze_x)) {
        modify_info.SetModified();
      }
      if (ImGui::Checkbox("Freeze Y", &rb.position_constraints.freeze_y)) {
        modify_info.SetModified();
      }
      if (ImGui::Checkbox("Freeze Z", &rb.position_constraints.freeze_z)) {
        modify_info.SetModified();
      }

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Rotation Constraints")) {

      if (ImGui::Checkbox("Freeze Pitch",
                          &rb.rotation_constraints.freeze_pitch)) {
        modify_info.SetModified();
      }
      if (ImGui::Checkbox("Freeze Yaw", &rb.rotation_constraints.freeze_yaw)) {
        modify_info.SetModified();
      }
      if (ImGui::Checkbox("Freeze Roll",
                          &rb.rotation_constraints.freeze_roll)) {
        modify_info.SetModified();
      }

      ImGui::TreePop();
    }
  });

  DrawComponent<BoxCollider>(
      "BoxCollider", selected_entity, [](BoxCollider& col) {
        if (ImGui::DragFloat3("Local Position",
                              glm::value_ptr(col.local_position), 0.1f)) {
          modify_info.SetModified();
        }

        if (ImGui::DragFloat3("Local Scale", glm::value_ptr(col.local_scale),
                              0.1f)) {
          modify_info.SetModified();
        }

        if (ImGui::Checkbox("Is Trigger", &col.is_trigger)) {
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

void DrawUniformField(ShaderUniform& uniform) {
  std::string uniform_name = uniform.name;

  std::visit(
      [&](auto&& value) {
        using ValueType = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<ValueType, float>) {
          if (ImGui::DragFloat(uniform_name.c_str(), &value)) {
            modify_info.SetModified();
          }
        } else if constexpr (std::is_same_v<ValueType, glm::vec2>) {
          if (ImGui::DragFloat2(uniform_name.c_str(), &value[0])) {
            modify_info.SetModified();
          }
        } else if constexpr (std::is_same_v<ValueType, glm::vec3>) {
          if (ImGui::DragFloat3(uniform_name.c_str(), &value[0])) {
            modify_info.SetModified();
          }
        } else if constexpr (std::is_same_v<ValueType, glm::vec4>) {
          if (ImGui::DragFloat4(uniform_name.c_str(), &value[0])) {
            modify_info.SetModified();
          }
        } else if constexpr (std::is_same_v<ValueType, int>) {
          if (ImGui::DragInt(uniform_name.c_str(), &value)) {
            modify_info.SetModified();
          }
        } else if constexpr (std::is_same_v<ValueType, bool>) {
          if (ImGui::Checkbox(uniform_name.c_str(), &value)) {
            modify_info.SetModified();
          }
        }
      },
      uniform.value);
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
    case ScriptFieldType::kColor: {
      Color color = !use_default ? script_field.GetValue<Color>() : Color(0.0f);
      if (ImGui::ColorEdit4(name.c_str(), &color.r)) {
        script_field.SetValue(color);
        modify_info.SetModified();
      }
      break;
    }
    case ScriptFieldType::kEntity: {
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
    case ScriptFieldType::kColor: {
      Color color = script_instance->GetFieldValue<Color>(name);
      if (ImGui::ColorEdit4(name.c_str(), &color.r)) {
        script_instance->SetFieldValue(name, color);
      }
      break;
    }
    case ScriptFieldType::kEntity: {
      break;
    }
  }
}
}  // namespace eve
