// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene_serializer.h"

#include <fstream>

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include "core/debug/assert.h"
#include "core/utils/guuid.h"
#include "scene/entity.h"

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

SceneSerializer::SceneSerializer(const Ref<Scene>& scene,
                                 const Ref<AssetLibrary>& asset_library)
    : scene_(scene), asset_library_(asset_library) {}

static void SerializeEntity(YAML::Emitter& out, Entity entity) {
  ENGINE_ASSERT(entity.HasComponent<IdComponent>())

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
        << ortho_camera.GetAspectRatio();
    out << YAML::Key << "zoom_level" << YAML::Value
        << ortho_camera.GetZoomLevel();
    out << YAML::Key << "near_clip" << YAML::Value
        << ortho_camera.GetNearClip();

    out << YAML::Key << "far_clip" << YAML::Value << ortho_camera.GetFarClip();
    out << YAML::EndMap;  // OrthographicCamera

    out << YAML::Key << "perspective_camera" << YAML::Value;
    out << YAML::BeginMap;  // PerspectiveCamera
    out << YAML::Key << "aspect_ratio" << YAML::Value
        << persp_camera.GetAspectRatio();
    out << YAML::Key << "fov" << YAML::Value << persp_camera.GetFov();
    out << YAML::Key << "near_clip" << YAML::Value
        << persp_camera.GetNearClip();
    out << YAML::Key << "far_clip" << YAML::Value << persp_camera.GetFarClip();
    out << YAML::EndMap;  // PerspectiveCamera

    out << YAML::Key << "is_orthographic" << YAML::Value
        << camera_component.is_orthographic;
    out << YAML::Key << "is_primary" << YAML::Value
        << camera_component.is_primary;
    out << YAML::Key << "is_fixed_aspect_ratio" << YAML::Value
        << camera_component.is_fixed_aspect_ratio;

    out << YAML::EndMap;  // CameraComponent
  }

  // TODO temporarly
  if (entity.HasComponent<DrawableComponent>()) {
    auto& drawable_component = entity.GetComponent<DrawableComponent>();

    out << YAML::Key << "drawable_component";
    out << YAML::BeginMap;  // DrawableComponent

    out << YAML::Key << "vertices" << YAML::Value << YAML::BeginSeq;
    for (auto vertex : drawable_component.packet.vertices) {
      out << YAML::BeginMap;
      out << YAML::Key << "position" << YAML::Value << vertex.position;
      out << YAML::Key << "color" << YAML::Value << vertex.color;
      out << YAML::Key << "tex_coords" << YAML::Value << vertex.tex_coords;
      out << YAML::Key << "tex_index" << YAML::Value << vertex.tex_index;
      out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "indices" << YAML::Value << YAML::BeginSeq;
    for (auto index : drawable_component.packet.indices) {
      out << YAML::Value << index;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "texture" << YAML::Value
        << drawable_component.texture->info.meta_path.string();

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
    LOG_ENGINE_ERROR("Failed to load scene file '{0}'\n\t{1}",
                     file_path.string(), e.what());
    return false;
  }

  if (!data["scene"]) {
    return false;
  }

  std::string scene_name = data["scene"].as<std::string>();
  LOG_ENGINE_TRACE("Deserializing scene: {0}", scene_name);

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
      camera_component.ortho_camera.SetAspectRatio(
          ortho_camera_yaml["aspect_ratio"].as<float>());
      camera_component.ortho_camera.SetZoomLevel(
          ortho_camera_yaml["zoom_level"].as<float>());
      camera_component.ortho_camera.SetNearClip(
          ortho_camera_yaml["near_clip"].as<float>());
      camera_component.ortho_camera.SetFarClip(
          ortho_camera_yaml["far_clip"].as<float>());

      auto persp_camera_yaml = camera_comp_yaml["perspective_camera"];

      camera_component.persp_camera.SetAspectRatio(
          persp_camera_yaml["aspect_ratio"].as<float>());
      camera_component.persp_camera.SetFov(
          persp_camera_yaml["fov"].as<float>());
      camera_component.persp_camera.SetNearClip(
          persp_camera_yaml["near_clip"].as<float>());
      camera_component.persp_camera.SetFarClip(
          persp_camera_yaml["far_clip"].as<float>());

      camera_component.is_orthographic =
          camera_comp_yaml["is_orthographic"].as<bool>();
      camera_component.is_primary = camera_comp_yaml["is_primary"].as<bool>();
      camera_component.is_fixed_aspect_ratio =
          camera_comp_yaml["is_fixed_aspect_ratio"].as<bool>();
    }

    auto drawable_comp_yaml = entity["drawable_component"];
    if (drawable_comp_yaml) {
      auto& drawable_component =
          deserialing_entity.AddComponent<DrawableComponent>();

      for (auto vertex_yaml : drawable_comp_yaml["vertices"]) {
        Vertex vertex;
        vertex.position = vertex_yaml["position"].as<glm::vec4>();
        vertex.color = vertex_yaml["color"].as<glm::vec4>();
        vertex.tex_coords = vertex_yaml["tex_coords"].as<glm::vec2>();
        vertex.tex_index = vertex_yaml["tex_index"].as<float>();
        drawable_component.packet.vertices.push_back(vertex);
      }

      for (auto index_yaml : drawable_comp_yaml["indices"]) {
        drawable_component.packet.indices.push_back(index_yaml.as<uint32_t>());
      }

      drawable_component.texture = asset_library_->LoadFromMeta<Texture>(
          drawable_comp_yaml["texture"].as<std::string>());
    }
  }

  return true;
}
