// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "physics/box_collider.h"

namespace eve {

template <>
bool ColliderIntersects(const Transform& lhs_transform,
                        const BoxCollider& lhs_collider,
                        const Transform& rhs_transform,
                        const BoxCollider& rhs_collider) {
  // Get world-space properties
  glm::vec3 lhs_position =
      lhs_transform.GetPosition() + lhs_collider.local_position;
  glm::vec3 lhs_scale = lhs_collider.local_scale;
  glm::vec3 rhs_position =
      rhs_transform.GetPosition() + rhs_collider.local_position;
  glm::vec3 rhs_scale = rhs_collider.local_scale;

  // Check for intersection along each axis
  bool x_intersects = std::abs(lhs_position.x - rhs_position.x) <
                      (lhs_scale.x + rhs_scale.x) / 2.0f;
  bool y_intersects = std::abs(lhs_position.y - rhs_position.y) <
                      (lhs_scale.y + rhs_scale.y) / 2.0f;
  bool z_intersects = std::abs(lhs_position.z - rhs_position.z) <
                      (lhs_scale.z + rhs_scale.z) / 2.0f;

  // The boxes intersect if and only if they intersect along all axes
  return x_intersects && y_intersects && z_intersects;
}

}  // namespace eve
