// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene.h"

#include "core/math/transform.h"
#include "graphics/camera.h"
#include "scene/entity.h"

Scene::Scene(Ref<State>& state) : state_(state) {}

Entity Scene::CreateEntity(const std::string& name) {
  return CreateEntityWithUUID(GUUID(), name);
}

Entity Scene::CreateEntityWithUUID(GUUID uuid, const std::string& name) {
  Entity entity = {registry_.create(), this};
  entity.AddComponent<IDComponent>(uuid);
  entity.AddComponent<Transform>();
  auto& tag = entity.AddComponent<TagComponent>();
  tag.tag = name.empty() ? "Entity" : name;

  entity_map_[uuid] = entity;

  return entity;
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

  Camera* main_camera = nullptr;
  Transform* camera_transform = nullptr;
  auto view = registry_.view<Transform, CameraComponent>();
  for (auto entity : view) {
    auto [transform, camera] = view.get<Transform, CameraComponent>(entity);

    if (camera.primary) {
      main_camera = camera.camera;
      camera_transform = &transform;
      break;
    }
  }

  if (main_camera && camera_transform) {
    Ref<Renderer>& renderer = state_->renderer;

    renderer->BeginScene({main_camera->GetViewMatrix(*camera_transform),
                          main_camera->GetProjectionMatrix()});

    registry_.view<Transform, DrawableComponent>().each(
        [renderer](entt::entity entity_id, Transform& tc,
                   DrawableComponent& dc) {
          for (auto& vertex : dc.packet.vertices) {
            vertex.position = tc.GetModelMatrix() * vertex.position;
          }

          if (dc.texture) {
            renderer->Draw(dc.packet, dc.texture);
          } else {
            renderer->Draw(dc.packet);
          }
        });

    renderer->EndScene();
  }
}

void Scene::OnUpdateEditor(float ds, EditorCamera& camera) {
  RenderScene(camera);
}

void Scene::OnViewportResize(glm::uvec2 size) {
  if (viewport_size_ == size) {
    return;
  }

  viewport_size_ = size;

  // Resize our non-FixedAspectRatio cameras
  registry_.view<CameraComponent>().each(
      [size](entt::entity, CameraComponent cc) {
        if (!cc.fixed_aspect_ratio)
          cc.camera->SetAspectRatio((float)size.x / (float)size.y);
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
    if (camera.primary)
      return std::optional<Entity>(Entity{entity, this});
  }
  return std::optional<Entity>();
}

void Scene::Step(int frames) {
  step_frames_ = frames;
}

void Scene::RenderScene(EditorCamera& camera) {
  Ref<Renderer>& renderer = state_->renderer;

  renderer->BeginScene({camera.GetViewMatrix(), camera.GetProjectionMatrix()});

  registry_.view<Transform, DrawableComponent>().each(
      [renderer](entt::entity entity_id, Transform& tc, DrawableComponent& dc) {
        for (auto& vertex : dc.packet.vertices) {
          vertex.position = tc.GetModelMatrix() * vertex.position;
        }

        if (dc.texture) {
          renderer->Draw(dc.packet, dc.texture);
        } else {
          renderer->Draw(dc.packet);
        }
      });

  renderer->EndScene();
}
