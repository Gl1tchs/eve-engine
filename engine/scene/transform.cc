// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/transform.h"

namespace eve {

glm::vec3 Transform::GetPosition() const {
  if (parent) {
    return local_position + parent->GetPosition();
  } else {
    return local_position;
  }
}

glm::vec3 Transform::GetRotation() const {
  if (parent) {
    return local_rotation + parent->GetRotation();
  } else {
    return local_rotation;
  }
}

glm::vec3 Transform::GetScale() const {
  if (parent) {
    return local_scale * parent->GetScale();
  } else {
    return local_scale;
  }
}

void Transform::Translate(glm::vec3 translation) {
  local_position += translation;
}

void Transform::Rotate(const float angle, const glm::vec3 axis) {
  local_rotation += angle * axis;
}

void Transform::LookAt(const glm::vec3& target) {
  glm::vec3 direction = glm::normalize(target - local_position);

  // Compute pitch and yaw angles using trigonometry
  float pitch = glm::degrees(asinf(-direction.y));
  float yaw = glm::degrees(atan2f(-direction.x, -direction.z));

  local_rotation = glm::vec3(pitch, yaw, 0.0f);
}

glm::vec3 Transform::GetForward() const {
  glm::fquat orientation = glm::fquat(glm::radians(local_rotation));
  return glm::normalize(orientation * kVec3Forward);
}

glm::vec3 Transform::GetRight() const {
  glm::fquat orientation = glm::fquat(glm::radians(local_rotation));
  return glm::normalize(orientation * kVec3Right);
}

glm::vec3 Transform::GetUp() const {
  glm::fquat orientation = glm::fquat(glm::radians(local_rotation));
  return glm::normalize(orientation * kVec3Up);
}

glm::mat4 Transform::GetTransformMatrix() const {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), local_position);
  transform =
      glm::rotate(transform, glm::radians(local_rotation.x), kVec3Right);
  transform = glm::rotate(transform, glm::radians(local_rotation.y), kVec3Up);
  transform =
      glm::rotate(transform, glm::radians(local_rotation.z), kVec3Forward);
  transform = glm::scale(transform, local_scale);

  if (parent) {
    transform = parent->GetTransformMatrix() * transform;
  }

  return transform;
}

glm::vec3 Transform::GetDirection() const {
  glm::vec3 direction(cos(local_rotation.x) * cos(local_rotation.y),
                      sin(local_rotation.x),
                      cos(local_rotation.x) * sin(local_rotation.y));
  direction = glm::normalize(direction);
  return direction;
}

}  // namespace eve
