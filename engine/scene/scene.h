// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include <entt/entt.hpp>

#include "core/state.h"
#include "core/uuid.h"
#include "scene/system.h"

namespace eve {

class Entity;

struct EntityCreateInfo {
  std::string name = "";
  UUID parent_id = 0;
};

class Scene {
 public:
  Scene(Ref<State> state, std::string name = "untitled");
  ~Scene();

  // Runtime
  bool OnRuntimeStart();

  void OnUpdateRuntime(float ds);

  void OnRuntimeStop();

  void OnUpdateEditor(float ds);

  [[nodiscard]] bool IsRunning() const { return is_running_; }
  [[nodiscard]] bool IsPaused() const { return is_paused_; }

  void SetPaused(bool paused) { is_paused_ = paused; }

  void Step(int frames = 1);

  [[nodiscard]] const std::string& GetName() const { return name_; }

  // Entities
  Entity CreateEntity(const EntityCreateInfo& info = {});
  Entity CreateEntityWithUUID(UUID uuid, const EntityCreateInfo& info = {});

  void DestroyEntity(Entity entity);

  [[nodiscard]] bool Exists(Entity entity);

  [[nodiscard]] Entity TryGetEntityByUUID(UUID uuid);
  [[nodiscard]] Entity TryGetEntityByName(std::string name);

  [[nodiscard]] Entity GetPrimaryCameraEntity();

  [[nodiscard]] Entity GetSelectedEntity();

  template <typename... Components>
  [[nodiscard]] std::map<UUID, Entity>& GetAllEntities() {
    return entity_map_;
  }

  template <typename... Components>
  [[nodiscard]] auto GetAllEntitiesWith() {
    return registry_.view<Components...>();
  }

  // Systems
  template <typename T, typename... Args>
    requires std::is_base_of_v<System, T>
  void PushSystem(Args&&... args) {
    T* system = new T(std::forward<Args>(args)...);
    system->scene_ = this;
    systems_.push_back(system);
  }

  static Ref<Scene> Copy(Ref<Scene> other);

 private:
  bool EntityNameExists(const std::string& name);

  [[nodiscard]] std::string GetEntityName(const std::string& name);

 private:
  Ref<State> state_;

  // ECS stuff
  entt::registry registry_;
  std::map<UUID, Entity> entity_map_;

  std::vector<System*> systems_;

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
