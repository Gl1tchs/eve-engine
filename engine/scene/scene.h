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
  Scene(Ref<State>& state, std::string name = "untitled");
  ~Scene() = default;

  static Ref<Scene> Copy(Ref<Scene> other);

  Entity CreateEntity(const std::string& name = std::string());
  Entity CreateEntityWithUUID(GUUID uuid,
                              const std::string& name = std::string());

  [[nodiscard]] bool Exists(Entity entity);

  void DestroyEntity(Entity entity);

  void OnRuntimeStart();
  void OnRuntimeStop();
  void OnUpdateRuntime(float ds);

  void OnUpdateEditor(float ds, EditorCamera& camera);
  void OnViewportResize(glm::uvec2 size);

  [[nodiscard]] std::optional<Entity> FindEntityByName(std::string_view name);
  [[nodiscard]] std::optional<Entity> FindEntityByUUID(GUUID uuid);

  [[nodiscard]] std::optional<Entity> GetPrimaryCameraEntity();

  [[nodiscard]] bool IsRunning() const { return is_running_; }
  [[nodiscard]] bool IsPaused() const { return is_paused_; }

  void SetPaused(bool paused) { is_paused_ = paused; }

  void Step(int frames = 1);

  template <typename... Components>
  [[nodiscard]] auto GetAllEntitiesWith() {
    return registry_.view<Components...>();
  }

  [[nodiscard]] const std::string& GetName() const { return name_; }

 private:
  void RenderScene(Camera& camera, Transform& transform);

  bool EntityNameExists(const std::string& name);

 private:
  entt::registry registry_;

  glm::uvec2 viewport_size_;

  bool is_running_ = false;
  bool is_paused_ = false;
  int step_frames_ = 0;

  Ref<State> state_;

  std::unordered_map<GUUID, Entity> entity_map_;

  std::string name_;

  friend class Entity;
  friend class SceneSerializer;
  friend class HierarchyPanel;
};
