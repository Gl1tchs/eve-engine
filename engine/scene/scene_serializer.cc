// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_serializer.h"

#include <nlohmann/json.hpp>

#include "asset/asset_registry.h"
#include "core/uuid.h"
#include "physics/rigidbody.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scripting/script.h"
#include "scripting/script_engine.h"

using json = nlohmann::json;

#define WRITE_SCRIPT_FIELD(FieldType, Type)                    \
  case ScriptFieldType::FieldType:                             \
    script_field_json["data"] = script_field.GetValue<Type>(); \
    break

#define READ_SCRIPT_FIELD(FieldType, Type)             \
  case ScriptFieldType::FieldType: {                   \
    Type data = script_field_json["data"].get<Type>(); \
    field_instance.SetValue(data);                     \
    break;                                             \
  }

namespace glm {
void to_json(json& j, const glm::vec2& vec) {
  j.push_back(vec.x);
  j.push_back(vec.y);
}

void from_json(const json& j, glm::vec2& vec) {
  j[0].get_to(vec.x);
  j[1].get_to(vec.y);
}

void to_json(json& j, const glm::vec3& vec) {
  j.push_back(vec.x);
  j.push_back(vec.y);
  j.push_back(vec.z);
}

void from_json(const json& j, glm::vec3& vec) {
  j[0].get_to(vec.x);
  j[1].get_to(vec.y);
  j[2].get_to(vec.z);
}

void to_json(json& j, const glm::vec4& vec) {
  j.push_back(vec.x);
  j.push_back(vec.y);
  j.push_back(vec.z);
  j.push_back(vec.w);
}

void from_json(const json& j, glm::vec4& vec) {
  j[0].get_to(vec.x);
  j[1].get_to(vec.y);
  j[3].get_to(vec.z);
  j[3].get_to(vec.w);
}
}  // namespace glm

namespace eve {

void to_json(json& j, const UUID& id) {
  j = (uint64_t)id;
}

void from_json(const json& j, UUID& id) {
  id = j.get<uint64_t>();
}

void to_json(json& j, const Color& color) {
  j.push_back(color.r);
  j.push_back(color.g);
  j.push_back(color.b);
  j.push_back(color.a);
}

void from_json(const json& j, Color& color) {
  j[0].get_to(color.r);
  j[1].get_to(color.g);
  j[2].get_to(color.b);
  j[3].get_to(color.a);
}

SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : scene_(scene) {}

static void SerializeEntity(json& out, Entity entity) {
  bool has_required_components =
      entity.HasComponent<IdComponent, TagComponent, RelationComponent,
                          Transform>();
  EVE_ASSERT_ENGINE(has_required_components);

  out["id"] = entity.GetUUID();

  RelationComponent& relation = entity.GetComponent<RelationComponent>();
  out["parent_id"] = relation.parent_id;

  out["tag"] = entity.GetComponent<TagComponent>().tag;

  if (entity.HasComponent<Transform>()) {
    auto& tc = entity.GetComponent<Transform>();

    json transform_json = {{"position", tc.local_position},
                           {"rotation", tc.local_rotation},
                           {"scale", tc.local_scale}};

    out["transform"] = transform_json;
  }

  if (entity.HasComponent<CameraComponent>()) {
    auto& camera_component = entity.GetComponent<CameraComponent>();

    out["camera_component"] =
        json{{"orthographic_camera",
              {{"aspect_ratio", camera_component.ortho_camera.aspect_ratio},
               {"zoom_level", camera_component.ortho_camera.zoom_level},
               {"near_clip", camera_component.ortho_camera.near_clip},
               {"far_clip", camera_component.ortho_camera.far_clip}}},
             {"perspective_camera",
              {{"aspect_ratio", camera_component.persp_camera.aspect_ratio},
               {"fov", camera_component.persp_camera.fov},
               {"near_clip", camera_component.persp_camera.near_clip},
               {"far_clip", camera_component.persp_camera.far_clip}}},
             {"is_orthographic", camera_component.is_orthographic},
             {"is_primary", camera_component.is_primary},
             {"is_fixed_aspect_ratio", camera_component.is_fixed_aspect_ratio}};
  }

  if (entity.HasComponent<SpriteRendererComponent>()) {
    auto& sprite_component = entity.GetComponent<SpriteRendererComponent>();

    out["sprite_component"] = json{{"texture", sprite_component.texture},
                                   {"color", sprite_component.color},
                                   {"tex_tiling", sprite_component.tex_tiling},
                                   {"tex_offset", sprite_component.tex_offset}};
  }

  if (entity.HasComponent<ModelComponent>()) {
    auto& model_component = entity.GetComponent<ModelComponent>();

    out["model_component"] = json{{"model", model_component.model}};
  }

  if (entity.HasComponent<Material>()) {
    auto& material = entity.GetComponent<Material>();

    out["material_component"] = json{{"albedo", material.albedo},
                                     {"shader", material.shader},
                                     {"uniform_fields", json::array()}};

    Ref<ShaderInstance> shader_instance =
        material.shader != 0
            ? AssetRegistry::Get<ShaderInstance>(material.shader)
            : nullptr;

    if (shader_instance) {
      json uniform_fields = json::array();
      for (auto& uniform : shader_instance->uniforms) {
        json uniform_json = {{"name", uniform.name},
                             {"type", ConvertUniformTypeToString(uniform.type)},
                             {"value", nullptr}};

        std::visit(
            [&](const auto& val) {
              using ValueType = std::decay_t<decltype(val)>;
              if constexpr (std::is_same_v<ValueType, float> ||
                            std::is_same_v<ValueType, glm::vec2> ||
                            std::is_same_v<ValueType, glm::vec3> ||
                            std::is_same_v<ValueType, glm::vec4> ||
                            std::is_same_v<ValueType, int> ||
                            std::is_same_v<ValueType, bool>) {
                uniform_json["value"] = val;
              }
            },
            uniform.value);

        uniform_fields.push_back(uniform_json);
      }

      out["material_component"]["uniform_fields"] = uniform_fields;
    }
  }

  if (entity.HasComponent<Rigidbody>()) {
    auto& rb = entity.GetComponent<Rigidbody>();

    out["rigidbody"] =
        json{{"velocity", rb.velocity},
             {"acceleration", rb.acceleration},
             {"mass", rb.mass},
             {"use_gravity", rb.use_gravity},
             {"position_constraints",
              json{{"freeze_x", rb.position_constraints.freeze_x},
                   {"freeze_y", rb.position_constraints.freeze_y},
                   {"freeze_z", rb.position_constraints.freeze_z}}},
             {"rotation_constraints",
              json{{"freeze_pitch", rb.rotation_constraints.freeze_pitch},
                   {"freeze_yaw", rb.rotation_constraints.freeze_yaw},
                   {"freeze_roll", rb.rotation_constraints.freeze_roll}}}};
  }

  if (entity.HasComponent<BoxCollider>()) {
    auto& col = entity.GetComponent<BoxCollider>();

    out["box_collider"] = json{{"is_trigger", col.is_trigger},
                               {"local_position", col.local_position},
                               {"local_scale", col.local_scale}};
  }

  if (entity.HasComponent<ScriptComponent>()) {
    auto& sc = entity.GetComponent<ScriptComponent>();

    json script_component_json = {{"class_name", sc.class_name},
                                  {"script_fields", json::array()}};

    Ref<ScriptClass> entity_class = ScriptEngine::GetEntityClass(sc.class_name);
    const auto& fields = entity_class->GetFields();
    if (!fields.empty()) {
      for (const auto& [name, field] : fields) {
        auto& entity_fields = ScriptEngine::GetScriptFieldMap(entity);

        if (entity_fields.find(name) != entity_fields.end()) {
          json script_field_json = {
              {"name", name},
              {"type", SerializeScriptField(field.type)},
              {"data", nullptr}  // Placeholder for the data
          };

          ScriptFieldInstance& script_field = entity_fields.at(name);

          switch (field.type) {
            WRITE_SCRIPT_FIELD(kFloat, float);
            WRITE_SCRIPT_FIELD(kDouble, double);
            WRITE_SCRIPT_FIELD(kBool, bool);
            WRITE_SCRIPT_FIELD(kChar, char);
            WRITE_SCRIPT_FIELD(kByte, int8_t);
            WRITE_SCRIPT_FIELD(kShort, int16_t);
            WRITE_SCRIPT_FIELD(kInt, int32_t);
            WRITE_SCRIPT_FIELD(kLong, int64_t);
            WRITE_SCRIPT_FIELD(kUByte, uint8_t);
            WRITE_SCRIPT_FIELD(kUShort, uint16_t);
            WRITE_SCRIPT_FIELD(kUInt, uint32_t);
            WRITE_SCRIPT_FIELD(kULong, uint64_t);
            WRITE_SCRIPT_FIELD(kVector2, glm::vec2);
            WRITE_SCRIPT_FIELD(kVector3, glm::vec3);
            WRITE_SCRIPT_FIELD(kVector4, glm::vec4);
            WRITE_SCRIPT_FIELD(kColor, Color);
            WRITE_SCRIPT_FIELD(kEntity, UUID);
            default:
              break;
          }

          script_component_json["script_fields"].push_back(script_field_json);
        }
      }
    }

    out["script_component"] = script_component_json;
  }
}

void SceneSerializer::Serialize(const fs::path& file_path) {
  json scene_json;
  scene_json["scene"] = file_path.stem().string();
  scene_json["entities"] = json::array();

  scene_->registry_.view<entt::entity>().each([&](auto entity_id) {
    Entity entity = {entity_id, scene_.get()};
    if (!entity)
      return;

    json entity_json;
    SerializeEntity(entity_json, entity);

    scene_json["entities"].push_back(entity_json);
  });

  std::ofstream fout(file_path);
  fout << scene_json.dump(2);
}

bool SceneSerializer::Deserialize(const fs::path& file_path) {
  json data;
  std::ifstream file(file_path);
  if (!file.is_open()) {
    EVE_LOG_ENGINE_ERROR("Failed to load scene file '{0}'", file_path.string());
    return false;
  }
  file >> data;

  if (!data.contains("scene")) {
    return false;
  }

  std::string scene_name = data["scene"].get<std::string>();
  EVE_LOG_ENGINE_TRACE("Deserializing scene: {0}", scene_name);

  scene_->name_ = scene_name;

  if (!data.contains("entities")) {
    return false;
  }

  json entities_json = data["entities"];

  // Create entities before adding components in order to build parent/child relations.
  std::vector<Entity> entities;
  for (const auto& entity_json : entities_json) {
    UUID uuid = entity_json["id"].get<UUID>();
    std::string name = entity_json["tag"].get<std::string>();
    entities.push_back(
        scene_->CreateEntityWithUUID(uuid, {name, kInvalidUUID}));
  }

  for (uint32_t i = 0; i < entities.size(); i++) {
    auto& entity_json = entities_json.at(i);
    auto& deserialing_entity = entities.at(i);

    // Set parent entity
    UUID parent_id = entity_json["parent_id"].get<UUID>();
    if (parent_id) {
      auto parent_entity = scene_->TryGetEntityByUUID(parent_id);
      if (parent_entity) {
        deserialing_entity.SetParent(parent_entity);
      }
    }

    if (auto transform_json = entity_json["transform"];
        !transform_json.is_null()) {
      auto& tc = deserialing_entity.GetComponent<Transform>();

      tc.local_position = transform_json["position"].get<glm::vec3>();
      tc.local_rotation = transform_json["rotation"].get<glm::vec3>();
      tc.local_scale = transform_json["scale"].get<glm::vec3>();
    }

    if (auto camera_comp_json = entity_json["camera_component"];
        !camera_comp_json.is_null()) {
      auto& camera_component =
          deserialing_entity.AddComponent<CameraComponent>();

      auto ortho_camera_json = camera_comp_json["orthographic_camera"];
      camera_component.ortho_camera.aspect_ratio =
          ortho_camera_json["aspect_ratio"].get<float>();
      camera_component.ortho_camera.zoom_level =
          ortho_camera_json["zoom_level"].get<float>();
      camera_component.ortho_camera.near_clip =
          ortho_camera_json["near_clip"].get<float>();
      camera_component.ortho_camera.far_clip =
          ortho_camera_json["far_clip"].get<float>();

      auto persp_camera_json = camera_comp_json["perspective_camera"];
      camera_component.persp_camera.aspect_ratio =
          persp_camera_json["aspect_ratio"].get<float>();
      camera_component.persp_camera.fov = persp_camera_json["fov"].get<float>();
      camera_component.persp_camera.near_clip =
          persp_camera_json["near_clip"].get<float>();
      camera_component.persp_camera.far_clip =
          persp_camera_json["far_clip"].get<float>();

      camera_component.is_orthographic =
          camera_comp_json["is_orthographic"].get<bool>();
      camera_component.is_primary = camera_comp_json["is_primary"].get<bool>();
      camera_component.is_fixed_aspect_ratio =
          camera_comp_json["is_fixed_aspect_ratio"].get<bool>();
    }

    if (auto sprite_comp_json = entity_json["sprite_component"];
        !sprite_comp_json.is_null()) {
      auto& sprite_component =
          deserialing_entity.AddComponent<SpriteRendererComponent>();

      sprite_component.texture = sprite_comp_json["texture"].get<UUID>();
      sprite_component.color = sprite_comp_json["color"].get<Color>();
      sprite_component.tex_tiling =
          sprite_comp_json["tex_tiling"].get<glm::vec2>();
      sprite_component.tex_offset =
          sprite_comp_json["tex_offset"].get<glm::vec2>();
    }

    if (auto model_comp_json = entity_json["model_component"];
        !model_comp_json.is_null()) {
      auto& model_component = deserialing_entity.AddComponent<ModelComponent>();

      model_component.model = model_comp_json["model"].get<UUID>();
    }

    if (auto material_json = entity_json["material_component"];
        !material_json.is_null()) {
      auto& material = deserialing_entity.AddComponent<Material>();

      material.albedo = material_json["albedo"].get<Color>();
      if (material_json.contains("shader")) {
        material.shader = material_json["shader"].get<UUID>();
      } else {
        material.shader = 0;
      }

      Ref<ShaderInstance> shader_instance =
          material.shader != 0
              ? AssetRegistry::Get<ShaderInstance>(material.shader)
              : nullptr;
      if (shader_instance) {
        auto uniforms_json = material_json["uniform_fields"];
        for (const auto& uniform_json : uniforms_json) {
          ShaderUniform uniform;
          uniform.name = uniform_json["name"].get<std::string>();
          uniform.type = ConvertStringToShaderUniformType(
              uniform_json["type"].get<std::string>());

          // necessary for std::visit
          uniform.value = GetDefaultShaderValue(uniform.type);
          std::visit(
              [&](auto& val) {
                using ValueType = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<ValueType, float> ||
                              std::is_same_v<ValueType, glm::vec2> ||
                              std::is_same_v<ValueType, glm::vec3> ||
                              std::is_same_v<ValueType, glm::vec4> ||
                              std::is_same_v<ValueType, int> ||
                              std::is_same_v<ValueType, bool>) {
                  val = uniform_json["value"].get<ValueType>();
                }
              },
              uniform.value);

          shader_instance->uniforms.push_back(uniform);
        }
      }
    }

    if (auto rigidbody_json = entity_json["rigidbody"];
        !rigidbody_json.is_null()) {
      auto& rb = deserialing_entity.AddComponent<Rigidbody>();

      rb.velocity = rigidbody_json["velocity"].get<glm::vec3>();
      rb.acceleration = rigidbody_json["acceleration"].get<glm::vec3>();
      rb.mass = rigidbody_json["mass"].get<float>();
      rb.use_gravity = rigidbody_json["use_gravity"].get<bool>();
      rb.position_constraints = {
          rigidbody_json["position_constraints"]["freeze_x"].get<bool>(),
          rigidbody_json["position_constraints"]["freeze_y"].get<bool>(),
          rigidbody_json["position_constraints"]["freeze_z"].get<bool>()};
      rb.rotation_constraints = {
          rigidbody_json["rotation_constraints"]["freeze_pitch"].get<bool>(),
          rigidbody_json["rotation_constraints"]["freeze_yaw"].get<bool>(),
          rigidbody_json["rotation_constraints"]["freeze_roll"].get<bool>()};
    }

    if (auto box_collider_json = entity_json["box_collider"];
        !box_collider_json.is_null()) {
      auto& col = deserialing_entity.AddComponent<BoxCollider>();

      col.is_trigger = box_collider_json["is_trigger"].get<bool>();
      col.local_position = box_collider_json["local_position"].get<glm::vec3>();
      col.local_scale = box_collider_json["local_scale"].get<glm::vec3>();
    }

    if (auto script_component_json = entity_json["script_component"];
        !script_component_json.is_null()) {
      auto& sc = deserialing_entity.AddComponent<ScriptComponent>();

      sc.class_name = script_component_json["class_name"].get<std::string>();

      auto script_fields_json = script_component_json["script_fields"];
      if (!script_fields_json.is_null()) {
        Ref<ScriptClass> entity_class =
            ScriptEngine::GetEntityClass(sc.class_name);
        if (entity_class) {
          const auto& fields = entity_class->GetFields();
          auto& entity_fields =
              ScriptEngine::GetScriptFieldMap(deserialing_entity);

          for (const auto& script_field_json : script_fields_json) {
            std::string name = script_field_json["name"].get<std::string>();
            std::string type_string =
                script_field_json["type"].get<std::string>();
            ScriptFieldType type = DeserializeScriptField(type_string);

            ScriptFieldInstance& field_instance = entity_fields[name];

            EVE_ASSERT_ENGINE(fields.find(name) != fields.end());

            if (fields.find(name) == fields.end()) {
              continue;
            }

            field_instance.field = fields.at(name);

            switch (type) {
              READ_SCRIPT_FIELD(kFloat, float);
              READ_SCRIPT_FIELD(kDouble, double);
              READ_SCRIPT_FIELD(kBool, bool);
              READ_SCRIPT_FIELD(kChar, char);
              READ_SCRIPT_FIELD(kByte, int8_t);
              READ_SCRIPT_FIELD(kShort, int16_t);
              READ_SCRIPT_FIELD(kInt, int32_t);
              READ_SCRIPT_FIELD(kLong, int64_t);
              READ_SCRIPT_FIELD(kUByte, uint8_t);
              READ_SCRIPT_FIELD(kUShort, uint16_t);
              READ_SCRIPT_FIELD(kUInt, uint32_t);
              READ_SCRIPT_FIELD(kULong, uint64_t);
              READ_SCRIPT_FIELD(kVector2, glm::vec2);
              READ_SCRIPT_FIELD(kVector3, glm::vec3);
              READ_SCRIPT_FIELD(kVector4, glm::vec4);
              READ_SCRIPT_FIELD(kColor, Color);
              READ_SCRIPT_FIELD(kEntity, UUID);
              default:
                break;
            }
          }
        }
      }
    }
  }

  return true;
}
}  // namespace eve
