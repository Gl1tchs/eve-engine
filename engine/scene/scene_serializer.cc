// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_serializer.h"

#include <yaml-cpp/yaml.h>

#include "asset/asset_library.h"
#include "core/utils/guuid.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scripting/script.h"
#include "scripting/script_engine.h"
#include "yaml-cpp/emittermanip.h"

#define WRITE_SCRIPT_FIELD(FieldType, Type) \
  case ScriptFieldType::FieldType:          \
    out << script_field.GetValue<Type>();   \
    break

#define READ_SCRIPT_FIELD(FieldType, Type)       \
  case ScriptFieldType::FieldType: {             \
    Type data = script_field["data"].as<Type>(); \
    field_instance.SetValue(data);               \
    break;                                       \
  }

namespace YAML {

template <>
struct convert<glm::vec2> {
  static Node encode(const glm::vec2& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec2& rhs) {
    if (!node.IsSequence() || node.size() != 2)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    return true;
  }
};

template <>
struct convert<glm::vec3> {
  static Node encode(const glm::vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec3& rhs) {
    if (!node.IsSequence() || node.size() != 3)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }
};

template <>
struct convert<glm::vec4> {
  static Node encode(const glm::vec4& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec4& rhs) {
    if (!node.IsSequence() || node.size() != 4)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
  }
};

template <>
struct convert<GUUID> {
  static Node encode(const GUUID& uuid) {
    Node node;
    node.push_back((uint64_t)uuid);
    return node;
  }

  static bool decode(const Node& node, GUUID& uuid) {
    uuid = node.as<uint64_t>();
    return true;
  }
};

}  // namespace YAML

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : scene_(scene) {}

static void SerializeEntity(YAML::Emitter& out, Entity entity) {
  ASSERT(entity.HasComponent<IdComponent>())

  out << YAML::BeginMap;
  out << YAML::Key << "id" << YAML::Value << entity.GetUUID();

  if (entity.HasComponent<TagComponent>()) {
    auto& tag = entity.GetComponent<TagComponent>().tag;

    out << YAML::Key << "tag_component";
    out << YAML::BeginMap;  // TagComponent

    out << YAML::Key << "tag" << YAML::Value << tag;

    out << YAML::EndMap;  // TagComponent
  }

  if (entity.HasComponent<Transform>()) {
    auto& tc = entity.GetComponent<Transform>();

    out << YAML::Key << "transform";
    out << YAML::BeginMap;  // Transform

    out << YAML::Key << "position" << YAML::Value << tc.position;
    out << YAML::Key << "rotation" << YAML::Value << tc.rotation;
    out << YAML::Key << "scale" << YAML::Value << tc.scale;

    out << YAML::EndMap;  // Transform
  }

  if (entity.HasComponent<CameraComponent>()) {
    auto& camera_component = entity.GetComponent<CameraComponent>();

    out << YAML::Key << "camera_component";
    out << YAML::BeginMap;  // CameraComponent

    auto& ortho_camera = camera_component.ortho_camera;
    auto& persp_camera = camera_component.persp_camera;

    out << YAML::Key << "orthographic_camera" << YAML::Value;
    out << YAML::BeginMap;  // OrthographicCamera
    out << YAML::Key << "aspect_ratio" << YAML::Value
        << ortho_camera.aspect_ratio;
    out << YAML::Key << "zoom_level" << YAML::Value << ortho_camera.zoom_level;
    out << YAML::Key << "near_clip" << YAML::Value << ortho_camera.near_clip;

    out << YAML::Key << "far_clip" << YAML::Value << ortho_camera.far_clip;
    out << YAML::EndMap;  // OrthographicCamera

    out << YAML::Key << "perspective_camera" << YAML::Value;
    out << YAML::BeginMap;  // PerspectiveCamera
    out << YAML::Key << "aspect_ratio" << YAML::Value
        << persp_camera.aspect_ratio;
    out << YAML::Key << "fov" << YAML::Value << persp_camera.fov;
    out << YAML::Key << "near_clip" << YAML::Value << persp_camera.near_clip;
    out << YAML::Key << "far_clip" << YAML::Value << persp_camera.far_clip;
    out << YAML::EndMap;  // PerspectiveCamera

    out << YAML::Key << "is_orthographic" << YAML::Value
        << camera_component.is_orthographic;
    out << YAML::Key << "is_primary" << YAML::Value
        << camera_component.is_primary;
    out << YAML::Key << "is_fixed_aspect_ratio" << YAML::Value
        << camera_component.is_fixed_aspect_ratio;

    out << YAML::EndMap;  // CameraComponent
  }

  if (entity.HasComponent<ModelComponent>()) {
    auto& model_component = entity.GetComponent<ModelComponent>();

    out << YAML::Key << "model_component";
    out << YAML::BeginMap;

    out << YAML::Key << "path" << YAML::Value
        << model_component.model->info.GetAssetPath();

    out << YAML::EndMap;
  }

  if (entity.HasComponent<Material>()) {
    auto& material = entity.GetComponent<Material>();

    out << YAML::Key << "material_component";
    out << YAML::BeginMap;

    out << YAML::Key << "albedo" << YAML::Value << material.albedo;
    out << YAML::Key << "metallic" << YAML::Value << material.metallic;
    out << YAML::Key << "roughness" << YAML::Value << material.roughness;
    out << YAML::Key << "ao" << YAML::Value << material.ao;

    out << YAML::EndMap;
  }

  if (entity.HasComponent<ScriptComponent>()) {
    auto& sc = entity.GetComponent<ScriptComponent>();

    out << YAML::Key << "script_component";
    out << YAML::BeginMap;

    out << YAML::Key << "class_name" << YAML::Value << sc.class_name;

    Ref<ScriptClass> entity_class = ScriptEngine::GetEntityClass(sc.class_name);
    const auto& fields = entity_class->GetFields();
    if (fields.size() > 0) {
      out << YAML::Key << "script_fields" << YAML::Value;
      auto& entity_fields = ScriptEngine::GetScriptFieldMap(entity);
      out << YAML::BeginSeq;
      for (const auto& [name, field] : fields) {
        if (entity_fields.find(name) == entity_fields.end()) {
          continue;
        }

        out << YAML::BeginMap;  // ScriptField
        out << YAML::Key << "name" << YAML::Value << name;
        out << YAML::Key << "type" << YAML::Value
            << ScriptFieldTypeToString(field.type);

        out << YAML::Key << "data" << YAML::Value;
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
          WRITE_SCRIPT_FIELD(kScriptEntity, GUUID);
          default:
            break;
        }
        out << YAML::EndMap;  // ScriptFields
      }
      out << YAML::EndSeq;
    }

    out << YAML::EndMap;
  }

  out << YAML::EndMap;
}

void SceneSerializer::Serialize(const fs::path& file_path) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "scene" << YAML::Value << file_path.stem().string();
  out << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
  scene_->registry_.view<entt::entity>().each([&](auto entity_id) {
    Entity entity = {entity_id, scene_.get()};
    if (!entity)
      return;

    SerializeEntity(out, entity);
  });
  out << YAML::EndSeq;
  out << YAML::EndMap;

  std::ofstream fout(file_path);
  fout << out.c_str();
}

bool SceneSerializer::Deserialize(const fs::path& file_path) {
  YAML::Node data;
  try {
    data = YAML::LoadFile(file_path.string());
  } catch (YAML::ParserException e) {
    LOG_ERROR("Failed to load scene file '{0}'\n\t{1}", file_path.string(),
              e.what());
    return false;
  }

  if (!data["scene"]) {
    return false;
  }

  std::string scene_name = data["scene"].as<std::string>();
  LOG_TRACE("Deserializing scene: {0}", scene_name);

  scene_->name_ = scene_name;

  auto entities = data["entities"];
  if (!entities) {
    return false;
  }

  for (auto entity : entities) {
    uint64_t uuid = entity["id"].as<GUUID>();

    std::string name;
    if (entity["tag_component"]) {
      name = entity["tag_component"]["tag"].as<std::string>();
    }

    Entity deserialing_entity = scene_->CreateEntityWithUUID(uuid, name);

    auto transform_yaml = entity["transform"];
    if (transform_yaml) {
      auto& tc = deserialing_entity.GetComponent<Transform>();

      tc.position = transform_yaml["position"].as<glm::vec3>();
      tc.rotation = transform_yaml["rotation"].as<glm::vec3>();
      tc.scale = transform_yaml["scale"].as<glm::vec3>();
    }

    auto camera_comp_yaml = entity["camera_component"];
    if (camera_comp_yaml) {
      auto& camera_component =
          deserialing_entity.AddComponent<CameraComponent>();

      auto ortho_camera_yaml = camera_comp_yaml["orthographic_camera"];
      camera_component.ortho_camera.aspect_ratio =
          ortho_camera_yaml["aspect_ratio"].as<float>();
      camera_component.ortho_camera.zoom_level =
          ortho_camera_yaml["zoom_level"].as<float>();
      camera_component.ortho_camera.near_clip =
          ortho_camera_yaml["near_clip"].as<float>();
      camera_component.ortho_camera.far_clip =
          ortho_camera_yaml["far_clip"].as<float>();

      auto persp_camera_yaml = camera_comp_yaml["perspective_camera"];

      camera_component.persp_camera.aspect_ratio =
          persp_camera_yaml["aspect_ratio"].as<float>();
      camera_component.persp_camera.fov = persp_camera_yaml["fov"].as<float>();
      camera_component.persp_camera.near_clip =
          persp_camera_yaml["near_clip"].as<float>();
      camera_component.persp_camera.far_clip =
          persp_camera_yaml["far_clip"].as<float>();

      camera_component.is_orthographic =
          camera_comp_yaml["is_orthographic"].as<bool>();
      camera_component.is_primary = camera_comp_yaml["is_primary"].as<bool>();
      camera_component.is_fixed_aspect_ratio =
          camera_comp_yaml["is_fixed_aspect_ratio"].as<bool>();
    }

    auto model_comp_yaml = entity["model_component"];
    if (model_comp_yaml) {
      auto& model_component = deserialing_entity.AddComponent<ModelComponent>();

      model_component.model = AssetLibrary::LoadFromPath<Model>(
          model_comp_yaml["path"].as<std::string>());
    }

    auto material_yaml = entity["material_component"];
    if (material_yaml) {
      auto& material = deserialing_entity.AddComponent<Material>();

      material.albedo = material_yaml["albedo"].as<glm::vec3>();
      material.metallic = material_yaml["metallic"].as<float>();
      material.roughness = material_yaml["roughness"].as<float>();
      material.ao = material_yaml["ao"].as<float>();
    }

    auto script_component_yaml = entity["script_component"];
    if (script_component_yaml) {
      auto& sc = deserialing_entity.AddComponent<ScriptComponent>();

      sc.class_name = script_component_yaml["class_name"].as<std::string>();

      auto script_fields = script_component_yaml["script_fields"];
      if (script_fields) {
        Ref<ScriptClass> entity_class =
            ScriptEngine::GetEntityClass(sc.class_name);
        if (entity_class) {
          const auto& fields = entity_class->GetFields();
          auto& entity_fields =
              ScriptEngine::GetScriptFieldMap(deserialing_entity);

          for (auto script_field : script_fields) {
            std::string name = script_field["name"].as<std::string>();
            std::string type_string = script_field["type"].as<std::string>();
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
              READ_SCRIPT_FIELD(kScriptEntity, GUUID);
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
