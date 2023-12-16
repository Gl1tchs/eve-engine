// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene.h"

#include "scene/entity.h"
#include "scene/transform.h"

Scene::Scene(Ref<State>& state, std::string name)
    : state_(state), name_(name) {}

template <typename... Component>
static void CopyComponent(
    entt::registry& dst, entt::registry& src,
    const std::unordered_map<GUUID, entt::entity>& entt_map) {
  (
      [&]() {
        auto view = src.view<Component>();
        for (auto src_entity : view) {
          entt::entity dst_entity =
              entt_map.at(src.get<IdComponent>(src_entity).id);

          auto& src_component = src.get<Component>(src_entity);
          dst.emplace_or_replace<Component>(dst_entity, src_component);
        }
      }(),
      ...);
}

template <typename... Component>
static void CopyComponent(
    ComponentGroup<Component...>, entt::registry& dst, entt::registry& src,
    const std::unordered_map<GUUID, entt::entity>& entt_map) {
  CopyComponent<Component...>(dst, src, entt_map);
}

template <typename... Component>
static void CopyComponentIfExists(Entity dst, Entity src) {
  (
      [&]() {
        if (src.HasComponent<Component>())
          dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
      }(),
      ...);
}

template <typename... Component>
static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst,
                                  Entity src) {
  CopyComponentIfExists<Component...>(dst, src);
}

Ref<Scene> Scene::Copy(Ref<Scene> other) {
  Ref<Scene> new_scene = CreateRef<Scene>(other->state_, other->name_);

  new_scene->viewport_size_ = other->viewport_size_;

  auto& src_scene_registry = other->registry_;
  auto& st_scene_registry = new_scene->registry_;
  std::unordered_map<GUUID, entt::entity> entt_map;

  // Create entities in new scene
  auto id_view = src_scene_registry.view<IdComponent>();
  for (auto e : id_view) {
    GUUID uuid = src_scene_registry.get<IdComponent>(e).id;
    const auto& name = src_scene_registry.get<TagComponent>(e).tag;
    Entity newEntity = new_scene->CreateEntityWithUUID(uuid, name);
    entt_map[uuid] = (entt::entity)newEntity;
  }

  // Copy components (except IdComponent and TagComponent)
  CopyComponent(AllComponents{}, st_scene_registry, src_scene_registry,
                entt_map);

  return new_scene;
}

Entity Scene::CreateEntity(const std::string& name) {
  return CreateEntityWithUUID(GUUID(), name);
}

Entity Scene::CreateEntityWithUUID(GUUID uuid, const std::string& name) {
  Entity entity = {registry_.create(), this};
  entity.AddComponent<IdComponent>(uuid);
  entity.AddComponent<Transform>();
  auto& tag = entity.AddComponent<TagComponent>();

  static int default_counter = 0;
  if (!name.empty()) {

    std::string name_unique = name;
    int counter = 1;

    while (EntityNameExists(name_unique)) {
      name_unique = std::format("{0} ({1})", name, counter);
      counter++;
    }

    tag.tag = name_unique;

  } else if (name.empty() && default_counter == 0) {
    tag.tag = "Entity";
    default_counter++;
  } else {
    tag.tag = std::format("Entity ({0})", default_counter);
    default_counter++;
  }

  entity_map_[uuid] = entity;

  return entity;
}

bool Scene::Exists(Entity entity) {
  if (!entity.HasComponent<IdComponent>()) {
    return false;
  }

  return entity_map_.find(entity.GetUUID()) != entity_map_.end();
}

void Scene::DestroyEntity(Entity entity) {
  entity_map_.erase(entity.GetUUID());
  registry_.destroy(entity);
}

void Scene::OnRuntimeStart() {
  is_running_ = true;

  // TODO start scripting engine
}

void Scene::OnRuntimeStop() {
  is_running_ = false;

  // TODO stop scripting engine
}

void Scene::OnUpdateRuntime(float ds) {
  if (is_paused_ && step_frames_-- <= 0) {
    return;
  }

  // TODO update scripts

  auto camera = GetPrimaryCameraEntity();
  if (camera.has_value() && camera.value().HasComponent<CameraComponent>()) {
    auto& cc = camera.value().GetComponent<CameraComponent>();
    auto& tc = camera.value().GetTransform();

    CameraData data;
    if (cc.is_orthographic) {
      data = {cc.ortho_camera.GetViewMatrix(tc),
              cc.ortho_camera.GetProjectionMatrix(), tc.position};
    } else {
      data = {cc.persp_camera.GetViewMatrix(tc),
              cc.persp_camera.GetProjectionMatrix(), tc.position};
    }

    RenderSceneRuntime(data);

    last_primary_transform_ = tc;
    editor_primary_used_ = true;
  }
}

void Scene::OnUpdateEditor(float ds, EditorCamera& editor_camera,
                           bool use_primary_if_exists) {
  if (editor_primary_used_) {
    editor_camera.GetTransform() = last_primary_transform_;
  }

  CameraData data;
  if (use_primary_if_exists) {
    auto primary_camera = GetPrimaryCameraEntity();
    if (primary_camera.has_value() &&
        primary_camera.value().HasComponent<CameraComponent>()) {
      auto& cc = primary_camera.value().GetComponent<CameraComponent>();
      auto& tc = primary_camera.value().GetTransform();

      if (cc.is_orthographic) {
        data = {cc.ortho_camera.GetViewMatrix(tc),
                cc.ortho_camera.GetProjectionMatrix(), tc.position};
      } else {
        data = {cc.persp_camera.GetViewMatrix(tc),
                cc.persp_camera.GetProjectionMatrix(), tc.position};
      }

      last_primary_transform_ = tc;
      editor_primary_used_ = true;
    }
  } else {
    data = {editor_camera.GetViewMatrix(), editor_camera.GetProjectionMatrix(),
            editor_camera.GetTransform().position};

    editor_primary_used_ = false;
  }

  RenderSceneEditor(data);
}

void Scene::OnViewportResize(glm::uvec2 size) {
  if (viewport_size_ == size) {
    return;
  }

  viewport_size_ = size;

  // Resize our non-FixedAspectRatio cameras
  registry_.view<CameraComponent>().each(
      [size](entt::entity, CameraComponent& cc) {
        if (!cc.is_fixed_aspect_ratio) {
          cc.persp_camera.aspect_ratio = (float)size.x / (float)size.y;
          cc.ortho_camera.aspect_ratio = (float)size.x / (float)size.y;
        }
      });
}

std::optional<Entity> Scene::FindEntityByUUID(GUUID uuid) {
  if (entity_map_.find(uuid) != entity_map_.end()) {
    return std::optional<Entity>(Entity{entity_map_.at(uuid), this});
  }
  return std::optional<Entity>();
}

std::optional<Entity> Scene::FindEntityByName(std::string_view name) {
  auto view = registry_.view<TagComponent>();
  for (auto entity : view) {
    const TagComponent& tc = view.get<TagComponent>(entity);
    if (tc.tag == name)
      return std::optional<Entity>(Entity{entity, this});
  }
  return std::optional<Entity>();
}

std::optional<Entity> Scene::GetPrimaryCameraEntity() {
  auto view = registry_.view<CameraComponent>();
  for (auto entity : view) {
    const auto& camera = view.get<CameraComponent>(entity);
    if (camera.is_primary)
      return std::optional<Entity>(Entity{entity, this});
  }
  return std::optional<Entity>();
}

void Scene::Step(int frames) {
  step_frames_ = frames;
}

void Scene::RenderSceneEditor(const CameraData& data) {
  Ref<Renderer>& renderer = state_->renderer;

  renderer->BeginScene(data);

  // Draw grid
  {
    float grid_size = 5.0f;  // Adjust the size of the grid
    int num_lines = 101;  // Number of grid lines (odd to have the center line)

    // Draw the base grid lines along the X, Y, and Z axes with different colors
    renderer->DrawLine({-grid_size * num_lines / 2.0f, 0.0f, 0.0f},
                       {grid_size * num_lines / 2.0f, 0.0f, 0.0f},
                       {1.0f, 0.0f, 0.0f, 1.0f});  // X-axis (red)
    renderer->DrawLine({0.0f, -grid_size * num_lines / 2.0f, 0.0f},
                       {0.0f, grid_size * num_lines / 2.0f, 0.0f},
                       {0.0f, 1.0f, 0.0f, 1.0f});  // Y-axis (green)
    renderer->DrawLine({0.0f, 0.0f, -grid_size * num_lines / 2.0f},
                       {0.0f, 0.0f, grid_size * num_lines / 2.0f},
                       {0.0f, 0.0f, 1.0f, 1.0f});  // Z-axis (blue)

    // Draw sub-grids with a different color (gray)
    glm::vec4 sub_grid_color(0.5f, 0.5f, 0.5f, 1.0f);

    // Draw the sub-grid lines along the X-axis
    for (int i = -num_lines / 2; i <= num_lines / 2; ++i) {
      float x_pos = static_cast<float>(i) * grid_size;
      if (i == 0) {
        continue;
      }

      renderer->DrawLine({x_pos, 0.0f, -grid_size * num_lines / 2.0f},
                         {x_pos, 0.0f, grid_size * num_lines / 2.0f},
                         sub_grid_color);
    }

    // Draw the sub-grid lines along the Z-axis
    for (int i = -num_lines / 2; i <= num_lines / 2; ++i) {
      float z_pos = static_cast<float>(i) * grid_size;
      if (i == 0) {
        continue;
      }

      renderer->DrawLine({-grid_size * num_lines / 2.0f, 0.0f, z_pos},
                         {grid_size * num_lines / 2.0f, 0.0f, z_pos},
                         sub_grid_color);
    }
  }

  // Draw camera bounds if selected
  if (selected_entity_ != nullptr && *selected_entity_ &&
      selected_entity_->HasComponent<CameraComponent>()) {
    CameraComponent& cc = selected_entity_->GetComponent<CameraComponent>();
    Transform& tc = selected_entity_->GetTransform();

    glm::vec4 color(0.75f, 0.75f, 0.75f, 1.0f);

    // Check if the camera is perspective or orthographic
    if (!cc.is_orthographic) {
      PerspectiveCamera& camera = cc.persp_camera;

      // Perspective camera
      float fov = glm::radians(camera.fov);
      float aspect_ratio = camera.aspect_ratio;
      float near_plane = camera.near_clip;
      float far_plane = camera.far_clip;

      float near_height = 2.0f * near_plane * glm::tan(fov / 2.0f);
      float near_width = near_height * aspect_ratio;

      float far_height = 2.0f * far_plane * glm::tan(fov / 2.0f);
      float far_width = far_height * aspect_ratio;

      // Calculate points of the camera's frustum
      Box near_box = {
          .bottom_left = tc.position - 0.5f * near_height * tc.GetUp() -
                         0.5f * near_width * tc.GetRight() +
                         near_plane * tc.GetForward(),
          .bottom_right = tc.position - 0.5f * near_height * tc.GetUp() +
                          0.5f * near_width * tc.GetRight() +
                          near_plane * tc.GetForward(),
          .top_left = tc.position + 0.5f * near_height * tc.GetUp() -
                      0.5f * near_width * tc.GetRight() +
                      near_plane * tc.GetForward(),
          .top_right = tc.position + 0.5f * near_height * tc.GetUp() +
                       0.5f * near_width * tc.GetRight() +
                       near_plane * tc.GetForward(),
      };

      Box far_box = {
          .bottom_left = tc.position - 0.5f * far_height * tc.GetUp() -
                         0.5f * far_width * tc.GetRight() +
                         far_plane * tc.GetForward(),
          .bottom_right = tc.position - 0.5f * far_height * tc.GetUp() +
                          0.5f * far_width * tc.GetRight() +
                          far_plane * tc.GetForward(),
          .top_left = tc.position + 0.5f * far_height * tc.GetUp() -
                      0.5f * far_width * tc.GetRight() +
                      far_plane * tc.GetForward(),
          .top_right = tc.position + 0.5f * far_height * tc.GetUp() +
                       0.5f * far_width * tc.GetRight() +
                       far_plane * tc.GetForward()};

      // Draw frustum boxes
      renderer->DrawBox(near_box, color);
      renderer->DrawBox(far_box, color);

      // Draw connection lines
      renderer->DrawLine(near_box.top_left, far_box.top_left, color);
      renderer->DrawLine(near_box.top_right, far_box.top_right, color);
      renderer->DrawLine(near_box.bottom_left, far_box.bottom_left, color);
      renderer->DrawLine(near_box.bottom_right, far_box.bottom_right, color);
    } else {
      OrthographicCamera& camera = cc.ortho_camera;

      // Orthographic camera
      float zoom_level = camera.zoom_level;
      float aspect_ratio = camera.aspect_ratio;
      float near_plane = camera.near_clip;
      float far_plane = camera.far_clip;

      float half_height = zoom_level * 0.5f;
      float half_width = half_height * aspect_ratio;

      // Calculate points of the camera's frustum
      Box near_box = {.bottom_left = tc.position - half_height * tc.GetUp() -
                                     half_width * tc.GetRight() +
                                     near_plane * tc.GetForward(),
                      .bottom_right = tc.position - half_height * tc.GetUp() +
                                      half_width * tc.GetRight() +
                                      near_plane * tc.GetForward(),
                      .top_left = tc.position + half_height * tc.GetUp() -
                                  half_width * tc.GetRight() +
                                  near_plane * tc.GetForward(),
                      .top_right = tc.position + half_height * tc.GetUp() +
                                   half_width * tc.GetRight() +
                                   near_plane * tc.GetForward()};

      Box far_box = {.bottom_left = tc.position - half_height * tc.GetUp() -
                                    half_width * tc.GetRight() +
                                    far_plane * tc.GetForward(),
                     .bottom_right = tc.position - half_height * tc.GetUp() +
                                     half_width * tc.GetRight() +
                                     far_plane * tc.GetForward(),
                     .top_left = tc.position + half_height * tc.GetUp() -
                                 half_width * tc.GetRight() +
                                 far_plane * tc.GetForward(),
                     .top_right = tc.position + half_height * tc.GetUp() +
                                  half_width * tc.GetRight() +
                                  far_plane * tc.GetForward()};

      // Draw frustum boxes
      renderer->DrawBox(near_box, color);
      renderer->DrawBox(far_box, color);

      // Draw connection lines
      renderer->DrawLine(near_box.top_left, far_box.top_left, color);
      renderer->DrawLine(near_box.top_right, far_box.top_right, color);
      renderer->DrawLine(near_box.bottom_left, far_box.bottom_left, color);
      renderer->DrawLine(near_box.bottom_right, far_box.bottom_right, color);
    }
  }

  RenderScene();

  renderer->EndScene();
}

void Scene::RenderSceneRuntime(const CameraData& data) {
  Ref<Renderer>& renderer = state_->renderer;

  renderer->BeginScene(data);

  RenderScene();

  renderer->EndScene();
}

void Scene::RenderScene() {
  Ref<Renderer>& renderer = state_->renderer;

  registry_.view<DirectionalLight, Transform>().each(
      [renderer](entt::entity entity_id, const DirectionalLight& light,
                 Transform& transform) {
        renderer->AddLight(light, transform.GetDirection());
      });

  registry_.view<Transform, ModelComponent>().each(
      [&](entt::entity entity_id, const Transform& transform,
          const ModelComponent& model_comp) {
        std::optional<Material> material{};
        if (registry_.all_of<Material>(entity_id)) {
          material =
              std::optional<Material>(registry_.get<Material>(entity_id));
        }

        renderer->Draw(model_comp.model->asset, transform, material);
      });
}

bool Scene::EntityNameExists(const std::string& name) {
  auto it =
      std::find_if(entity_map_.begin(), entity_map_.end(), [&](auto& pair) {
        const auto& tag_comp =
            pair.second.template GetComponent<TagComponent>();
        return tag_comp.tag == name;
      });

  return it != entity_map_.end();
}