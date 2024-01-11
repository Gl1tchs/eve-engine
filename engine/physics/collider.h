// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/transform.h"

namespace eve {

struct Collider {
  bool is_trigger = false;

  virtual ~Collider() = default;
};

template <typename T>
  requires std::is_base_of_v<Collider, T>
bool ColliderIntersects(const Transform& lhs_transform, const T& lhs_collider,
                        const Transform& rhs_transform, const T& rhs_collider) {
  return false;
}

}  // namespace eve