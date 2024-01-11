// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

struct PositionConstraints {
  bool freeze_x = false;
  bool freeze_y = false;
  bool freeze_z = false;
};

struct RotationConstraints {
  bool freeze_pitch = false;
  bool freeze_yaw = false;
  bool freeze_roll = false;
};

enum class ForceMode { kForce, kImpulse };

struct Rigidbody {
  glm::vec3 velocity;
  glm::vec3 acceleration;

  float mass = 1.0f;

  bool use_gravity = false;

  PositionConstraints position_constraints{};
  RotationConstraints rotation_constraints{};

  void AddForce(glm::vec3 force, ForceMode mode = ForceMode::kForce);
};

}  // namespace eve