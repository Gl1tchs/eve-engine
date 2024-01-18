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
  return scene_->TryGetEntityByUUID(GetRelation().parent_id);
}

void Entity::SetParent(Entity parent) {
  // check if this is not the parent of "parent"
  if (IsParentRecursive(*this, parent)) {
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
    // move child to top level
    child.GetRelation().parent_id = kInvalidUUID;
    children_ids.erase(it);

    return true;
  }

  return false;
}

bool Entity::IsParentRecursive(Entity parent, Entity child) {
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
    return IsParentRecursive(parent, child_parent);
  }
}

}  // namespace eve