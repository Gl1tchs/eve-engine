// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "physics/collider.h"

namespace eve {

struct BoxCollider : Collider {
  glm::vec3 local_position = {0, 0, 0};
  glm::vec3 local_scale = {0, 0, 0};

  // TODO add other collider as parameter
  using TriggerFunc = void (*)(std::string other_name);
  TriggerFunc on_trigger = nullptr;
};

template <>
bool ColliderIntersects(const Transform& lhs_transform,
                        const BoxCollider& lhs_collider,
                        const Transform& rhs_transform,
                        const BoxCollider& rhs_collider);

}  // namespace eve
