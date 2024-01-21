// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/entity.h"
#include "entity.h"

namespace eve {

Entity::Entity(entt::entity handle, Scene* scene)
    : entity_handle_(handle), scene_(scene) {}

RelationComponent& Entity::GetRelation() {
  return GetComponent<RelationComponent>();
}

Entity Entity::GetParent() {
  const UUID parent_id = GetRelation().parent_id;
  return parent_id != kInvalidUUID ? scene_->TryGetEntityByUUID(parent_id)
                                   : kInvalidEntity;
}

void Entity::SetParent(Entity parent) {
  // check if this is not the parent of "parent"
  if (IsParentOfRecursive(*this, parent)) {
    return;
  }

  // Remove old parent if exists.
  Entity current_parent = GetParent();
  if (current_parent) {
    current_parent.RemoveChild(*this);
  }

  // If an invalid entity provided then carry this into top levels
  if (!parent) {
    return;
  }

  RelationComponent& parent_relation = parent.GetRelation();
  auto& parent_children_ids = parent_relation.children_ids;

  // Setup relations between components
  RelationComponent& relation = GetRelation();
  relation.parent_id = parent.GetUUID();
  parent_children_ids.push_back(GetUUID());

  // Make transform values relative to the new parent.
  Transform& transform = GetTransform();
  Transform& parent_transform = parent.GetTransform();

  transform.local_position =
      transform.GetPosition() - parent_transform.GetPosition();
  transform.local_rotation =
      transform.GetRotation() - parent_transform.GetRotation();
  transform.local_scale = transform.GetScale() / parent_transform.GetScale();

  transform.parent = &parent_transform;
}

bool Entity::IsParent() {
  return GetRelation().children_ids.size() > 0;
}

bool Entity::IsChild() {
  return GetRelation().parent_id != kInvalidUUID;
}

std::vector<Entity> eve::Entity::GetChildren() {
  // TODO optimize this seems not so fast right now.
  const std::vector<UUID>& children_ids = GetRelation().children_ids;

  std::vector<Entity> children;
  for (const auto& child_id : children_ids) {
    children.push_back(scene_->TryGetEntityByUUID(child_id));
  }

  return children;
}

bool Entity::RemoveChild(Entity child) {
  auto& children_ids = GetRelation().children_ids;

  const auto it =
      std::find(children_ids.begin(), children_ids.end(), child.GetUUID());

  if (it != children_ids.end()) {
    // Set local positions as the world position
    Transform& child_transform = child.GetTransform();
    child_transform.local_position = child_transform.GetPosition();
    child_transform.local_rotation = child_transform.GetRotation();
    child_transform.local_scale = child_transform.GetScale();

    child_transform.parent = nullptr;

    // move child to top level
    child.GetRelation().parent_id = kInvalidUUID;
    children_ids.erase(it);

    return true;
  }

  return false;
}

bool Entity::IsParentOfRecursive(Entity parent, Entity child) {
  if (!child) {
    return false;
  }

  // check recursively if child is one of parent's
  Entity child_parent = child.GetParent();

  // if top level return false
  if (child_parent == kInvalidEntity) {
    return false;
  }
  // if parents are same then we found it
  else if (child_parent == parent) {
    return true;
  }
  // otherwise call the function again
  else {
    return IsParentOfRecursive(parent, child_parent);
  }
}

const UUID& Entity::GetUUID() {
  return GetComponent<IdComponent>().id;
}

const std::string& Entity::GetName() {
  return GetComponent<TagComponent>().tag;
}

Transform& Entity::GetTransform() {
  return GetComponent<Transform>();
}

Entity::operator bool() const {
  return entity_handle_ != entt::null || scene_ != nullptr;
}

Entity::operator entt::entity() const {
  return entity_handle_;
}

bool Entity::operator==(const Entity& other) const {
  return entity_handle_ == other.entity_handle_ && scene_ == other.scene_;
}

bool Entity::operator!=(const Entity& other) const {
  return !(*this == other);
}

}  // namespace eve