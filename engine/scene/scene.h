// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include <entt/entt.hpp>

#include "core/state.h"
#include "core/uuid.h"

namespace eve {

class Entity;

struct EntityCreateInfo {
  std::string name = "";
  UUID parent_id = 0;
};

class Scene {
 public:
  Scene(Ref<State> state, std::string name = "untitled");
  ~Scene() = default;

  static Ref<Scene> Copy(Ref<Scene> other);

  // Runtime
  bool OnRuntimeStart();

  void OnRuntimeStop();

  void OnUpdateRuntime(float ds);

  [[nodiscard]] bool IsRunning() const { return is_running_; }
  [[nodiscard]] bool IsPaused() const { return is_paused_; }

  void SetPaused(bool paused) { is_paused_ = paused; }

  void Step(int frames = 1);

  // ECS
  Entity CreateEntity(const EntityCreateInfo& info = {});
  Entity CreateEntityWithUUID(UUID uuid, const EntityCreateInfo& info = {});

  [[nodiscard]] bool Exists(Entity entity);

  void DestroyEntity(Entity entity);

  [[nodiscard]] Entity TryGetEntityByUUID(UUID uuid);
  [[nodiscard]] Entity TryGetEntityByName(std::string name);

  [[nodiscard]] Entity GetPrimaryCameraEntity();

  template <typename... Components>
  [[nodiscard]] auto GetAllEntitiesWith() {
    return registry_.view<Components...>();
  }

  [[nodiscard]] const std::string& GetName() const { return name_; }

  [[nodiscard]] Entity GetSelectedEntity();

 private:
  bool EntityNameExists(const std::string& name);

  [[nodiscard]] std::string GetEntityName(const std::string& name);

 private:
  Ref<State> state_;

  // ECS stuff
  entt::registry registry_;
  std::map<UUID, Entity> entity_map_;

  // scene properties
  std::string name_;

  bool is_running_ = false;
  bool is_paused_ = false;
  int step_frames_ = 0;

  Entity* selected_entity_{nullptr};

  friend class Entity;
  friend class HierarchyPanel;
  friend class SceneSerializer;
};
}  // namespace eve
