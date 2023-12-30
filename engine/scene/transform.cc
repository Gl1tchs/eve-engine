// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/transform.h"

namespace eve {
void Transform::Translate(glm::vec3 translation) {
  position += translation;
}

void Transform::Rotate(const float angle, const glm::vec3 axis) {
  rotation += angle * axis;
}

glm::vec3 Transform::GetForward() const {
  glm::fquat orientation = glm::fquat(glm::radians(rotation));
  return glm::normalize(orientation * kVec3Forward);
}

glm::vec3 Transform::GetRight() const {
  glm::fquat orientation = glm::fquat(glm::radians(rotation));
  return glm::normalize(orientation * kVec3Right);
}

glm::vec3 Transform::GetUp() const {
  glm::fquat orientation = glm::fquat(glm::radians(rotation));
  return glm::normalize(orientation * kVec3Up);
}

glm::mat4 Transform::GetModelMatrix() const {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

  transform = glm::rotate(transform, glm::radians(rotation.x), kVec3Right);
  transform = glm::rotate(transform, glm::radians(rotation.y), kVec3Up);
  transform = glm::rotate(transform, glm::radians(rotation.z), kVec3Forward);

  transform = glm::scale(transform, scale);

  return transform;
}

glm::vec3 Transform::GetDirection() const {
  glm::vec3 dir(cos(rotation.x) * cos(rotation.y), sin(rotation.x),
                cos(rotation.x) * sin(rotation.y));
  dir = glm::normalize(dir);
  return dir;
}
}  // namespace eve
