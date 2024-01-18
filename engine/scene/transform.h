// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

const glm::vec3 kVec3Up(0.0f, 1.0f, 0.0f);
const glm::vec3 kVec3Down = -kVec3Up;

const glm::vec3 kVec3Right(1.0f, 0.0f, 0.0f);
const glm::vec3 kVec3Left = -kVec3Right;

const glm::vec3 kVec3Forward(0.0f, 0.0f, -1.0f);
const glm::vec3 kVec3Back = -kVec3Forward;

const glm::vec3 kVec3Zero(0.0f, 0.0f, 0.0f);
const glm::vec3 kVec3One(1.0f, 1.0f, 1.0f);

const glm::vec3 kWorldUp = kVec3Up;

struct Transform final {
  Transform* parent = nullptr;

  glm::vec3 local_position = kVec3Zero;
  glm::vec3 local_rotation = kVec3Zero;
  glm::vec3 local_scale = kVec3One;

  [[nodiscard]] glm::vec3 GetPosition() const;
  [[nodiscard]] glm::vec3 GetRotation() const;
  [[nodiscard]] glm::vec3 GetScale() const;

  void Translate(glm::vec3 translation);

  void Rotate(float angle, glm::vec3 axis);

  void LookAt(const glm::vec3& target);

  [[nodiscard]] glm::vec3 GetForward() const;

  [[nodiscard]] glm::vec3 GetRight() const;

  [[nodiscard]] glm::vec3 GetUp() const;

  [[nodiscard]] glm::mat4 GetTransformMatrix() const;

  [[nodiscard]] glm::vec3 GetDirection() const;
};

}  // namespace eve
