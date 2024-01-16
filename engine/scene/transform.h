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
  glm::vec3 position = kVec3Zero;
  glm::vec3 rotation = kVec3Zero;
  glm::vec3 scale = kVec3One;

  void Translate(glm::vec3 translation);

  void Rotate(float angle, glm::vec3 axis);

  void LookAt(const glm::vec3& target);

  [[nodiscard]] glm::vec3 GetForward() const;

  [[nodiscard]] glm::vec3 GetRight() const;

  [[nodiscard]] glm::vec3 GetUp() const;

  [[nodiscard]] glm::mat4 GetModelMatrix() const;

  [[nodiscard]] glm::vec3 GetDirection() const;
};

}  // namespace eve
