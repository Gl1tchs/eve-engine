// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_serializer.h"

#include <nlohmann/json.hpp>

#include "asset/asset_registry.h"
#include "core/uuid.h"
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
  ASSERT(entity.HasComponent<IdComponent>())

  out["id"] = entity.GetUUID();

  if (entity.HasComponent<TagComponent>()) {
    auto& tag = entity.GetComponent<TagComponent>().tag;

    out["tag_component"] = json{{"tag", tag}};
  }

  if (entity.HasComponent<Transform>()) {
    auto& tc = entity.GetComponent<Transform>();

    json transform_json = {{"position", tc.position},
                           {"rotation", tc.rotation},
                           {"scale", tc.scale}};

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

    out["material_component"] = json{
        {"albedo", material.albedo},       {"metallic", material.metallic},
        {"roughness", material.roughness}, {"ao", material.ao},
        {"shader", material.shader},       {"uniform_fields", json::array()}};

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
              {"type", ScriptFieldTypeToString(field.type)},
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
    LOG_ENGINE_ERROR("Failed to load scene file '{0}'", file_path.string());
    return false;
  }
  file >> data;

  if (!data.contains("scene")) {
    return false;
  }

  std::string scene_name = data["scene"].get<std::string>();
  LOG_ENGINE_TRACE("Deserializing scene: {0}", scene_name);

  scene_->name_ = scene_name;

  if (!data.contains("entities")) {
    return false;
  }

  json entities = data["entities"];
  for (auto entity : entities) {
    uint64_t uuid = entity["id"].get<UUID>();

    std::string name;
    if (entity.contains("tag_component")) {
      name = entity["tag_component"]["tag"].get<std::string>();
    }

    Entity deserialing_entity = scene_->CreateEntityWithUUID(uuid, name);

    if (auto transform_json = entity["transform"]; !transform_json.is_null()) {
      auto& tc = deserialing_entity.GetComponent<Transform>();

      tc.position = transform_json["position"].get<glm::vec3>();
      tc.rotation = transform_json["rotation"].get<glm::vec3>();
      tc.scale = transform_json["scale"].get<glm::vec3>();
    }

    if (auto camera_comp_json = entity["camera_component"];
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

    if (auto sprite_comp_json = entity["sprite_component"];
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

    if (auto model_comp_json = entity["model_component"];
        !model_comp_json.is_null()) {
      auto& model_component = deserialing_entity.AddComponent<ModelComponent>();

      model_component.model = model_comp_json["model"].get<UUID>();
    }

    if (auto material_json = entity["material_component"];
        !material_json.is_null()) {
      auto& material = deserialing_entity.AddComponent<Material>();

      material.albedo = material_json["albedo"].get<Color>();
      material.metallic = material_json["metallic"].get<float>();
      material.roughness = material_json["roughness"].get<float>();
      material.ao = material_json["ao"].get<float>();
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

    if (auto script_component_json = entity["script_component"];
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
            ScriptFieldType type = ScriptFieldTypeFromString(type_string);

            ScriptFieldInstance& field_instance = entity_fields[name];

            ASSERT(fields.find(name) != fields.end());

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
