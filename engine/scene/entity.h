// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include <entt/entt.hpp>

#include "core/uuid.h"
#include "scene/components.h"
#include "scene/scene.h"

namespace eve {

class Entity {
 public:
  Entity() = default;
  Entity(entt::entity handle, Scene* scene);
  Entity(const Entity& other) = default;

  template <typename T, typename... Args>
  auto& AddComponent(Args&&... args) {
    EVE_ASSERT_ENGINE(!HasComponent<T>(), "Entity already has that component!");
    return scene_->registry_.emplace<T>(entity_handle_,
                                        std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T& AddOrReplaceComponent(Args&&... args) {
    T& component = scene_->registry_.emplace_or_replace<T>(
        entity_handle_, std::forward<Args>(args)...);
    return component;
  }

  template <typename T>
  [[nodiscard]] T& GetComponent() {
    EVE_ASSERT_ENGINE(HasComponent<T>(), "Entity does not have component!");
    return scene_->registry_.get<T>(entity_handle_);
  }

  template <typename... Components>
  [[nodiscard]] bool HasComponent() const {
    return scene_->registry_.all_of<Components...>(entity_handle_);
  }

  template <typename T>
  void RemoveComponent() {
    EVE_ASSERT_ENGINE(HasComponent<T>(), "Entity does not have component!");
    scene_->registry_.remove<T>(entity_handle_);
  }

  // Relations
  [[nodiscard]] RelationComponent& GetRelation();

  [[nodiscard]] Entity GetParent();

  void SetParent(Entity parent);

  [[nodiscard]] bool IsParent();

  [[nodiscard]] bool IsChild();

  [[nodiscard]] std::vector<Entity> GetChildren();

  bool RemoveChild(Entity child);

  [[nodiscard]] static bool IsParentOfRecursive(Entity parent, Entity child);

  [[nodiscard]] const UUID& GetUUID();

  [[nodiscard]] const std::string& GetName();

  [[nodiscard]] Transform& GetTransform();

  operator bool() const;

  operator entt::entity() const;

  operator uint32_t() const { return (uint32_t)entity_handle_; }

  [[nodiscard]] bool operator==(const Entity& other) const;

  [[nodiscard]] bool operator!=(const Entity& other) const;

 private:
  entt::entity entity_handle_{entt::null};
  Scene* scene_ = nullptr;

  friend class HierarchyPanel;
  friend class Scene;
};

const Entity kInvalidEntity = Entity{};

}  // namespace eve
