// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <entt/entt.hpp>

#include "core/debug/assert.h"
#include "core/utils/guuid.h"
#include "scene/components.h"
#include "scene/scene.h"

class Entity {
 public:
  Entity() = default;
  Entity(entt::entity handle, Scene* scene);
  Entity(const Entity& other) = default;

  template <typename T, typename... Args>
  T& AddComponent(Args&&... args) {
    ENGINE_ASSERT(!HasComponent<T>(), "Entity already has that component!");
    T& component = scene_->registry_.emplace<T>(entity_handle_,
                                                std::forward<Args>(args)...);
    return component;
  }

  template <typename T, typename... Args>
  T& AddOrReplaceComponent(Args&&... args) {
    T& component = scene_->registry_.emplace_or_replace<T>(
        entity_handle_, std::forward<Args>(args)...);
    return component;
  }

  template <typename T>
  [[nodiscard]] T& GetComponent() {
    ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component!");
    return scene_->registry_.get<T>(entity_handle_);
  }

  template <typename... Components>
  [[nodiscard]] bool HasComponent() {
    return scene_->registry_.all_of<Components...>(entity_handle_);
  }

  template <typename T>
  void RemoveComponent() {
    ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component!");
    scene_->registry_.remove<T>(entity_handle_);
  }

  operator bool() const {
    return entity_handle_ != entt::null || scene_ != nullptr;
  }
  operator entt::entity() const { return entity_handle_; }
  operator uint32_t() const { return (uint32_t)entity_handle_; }

  [[nodiscard]] GUUID GetUUID() { return GetComponent<IdComponent>().id; }
  [[nodiscard]] const std::string& GetName() {
    return GetComponent<TagComponent>().tag;
  }

  [[nodiscard]] Transform& GetTransform() { return GetComponent<Transform>(); }

  [[nodiscard]] bool operator==(const Entity& other) const {
    return entity_handle_ == other.entity_handle_ && scene_ == other.scene_;
  }

  [[nodiscard]] bool operator!=(const Entity& other) const {
    return !(*this == other);
  }

 private:
  entt::entity entity_handle_{entt::null};
  Scene* scene_ = nullptr;
};
