// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <optional>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/state.h"
#include "core/utils/guuid.h"
#include "scene/editor_camera.h"

class Entity;

class Scene {
 public:
  Scene(Ref<State>& state);
  ~Scene() = default;

  Entity CreateEntity(const std::string& name = std::string());
  Entity CreateEntityWithUUID(GUUID uuid,
                              const std::string& name = std::string());

  void DestroyEntity(Entity entity);

  void OnRuntimeStart();
  void OnRuntimeStop();
  void OnUpdateRuntime(float ds);

  void OnUpdateEditor(float ds, EditorCamera& camera);
  void OnViewportResize(glm::uvec2 size);

  std::optional<Entity> FindEntityByName(std::string_view name);
  std::optional<Entity> FindEntityByUUID(GUUID uuid);

  std::optional<Entity> GetPrimaryCameraEntity();

  bool IsRunning() const { return is_running_; }
  bool IsPaused() const { return is_paused_; }

  void SetPaused(bool paused) { is_paused_ = paused; }

  void Step(int frames = 1);

  template <typename... Components>
  auto GetAllEntitiesWith() {
    return registry_.view<Components...>();
  }

 private:
  void RenderScene(EditorCamera& camera);

 private:
  entt::registry registry_;
  glm::uvec2 viewport_size_;
  bool is_running_ = false;
  bool is_paused_ = false;
  int step_frames_ = 0;
  Ref<State> state_;

  std::unordered_map<GUUID, entt::entity> entity_map_;

  friend class Entity;
};
