// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_serializer.h"

#include <yaml-cpp/yaml.h>

#include "core/utils/guuid.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scripting/script_engine.h"
#include "yaml-cpp/emittermanip.h"

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

    out << YAML::Key << "path" << YAML::Value << sc.path;

    if (Ref<Script> script = sc.instance;
        script && !script->GetSerializeMap().empty()) {
      const auto& serialize_data = script->GetSerializeMap();

      out << YAML::Key << "fields" << YAML::Value << YAML::BeginSeq;

      for (const auto& [name, data] : serialize_data) {
        if (!data.is_overrided) {
          continue;
        }

        out << YAML::BeginMap;

        std::string result;
        std::visit(
            [&](const auto& arg) {
              using T = std::decay_t<decltype(arg)>;
              if constexpr (!std::is_same_v<T, std::string>) {
                result = std::to_string(arg);
              } else {
                result = arg;
              }
            },
            data.value);

        out << YAML::Key << "name" << YAML::Value << name;
        out << YAML::Key << "value" << YAML::Value << result;

        out << YAML::EndMap;
      }

      out << YAML::EndSeq;
    }

    out << YAML::EndMap;
  }

  out << YAML::EndMap;
}

void SceneSerializer::Serialize(const std::filesystem::path& file_path) {
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

bool SceneSerializer::Deserialize(const std::filesystem::path& file_path) {
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

      sc.path = script_component_yaml["path"].as<std::string>();
      sc.instance = ScriptEngine::CreateScript(sc.path);

      if (Ref<Script>& script = sc.instance; script) {
        auto fields_yaml = script_component_yaml["fields"];
        for (auto field_yaml : fields_yaml) {
          std::string name = field_yaml["name"].as<std::string>();
          std::string value_str = field_yaml["value"].as<std::string>();

          ScriptDataType value{};
          if (IsFloat(value_str)) {
            float value_float = std::stof(value_str);
            if (IsInteger(value_float)) {
              value = (int)value_float;
            } else {
              value = value_float;
            }
          } else {
            value = value_str;
          }

          script->SetSerializeDataField(name, value);
        }
      }
    }
  }

  return true;
}
