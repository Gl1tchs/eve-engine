// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/math/transform.h"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : position(position),
      rotation(rotation),
      scale(scale),
      forward_(kVec3Forward),
      right_(kVec3Right),
      up_(kVec3Up) {
  UpdateVectors();
}

void Transform::Translate(glm::vec3 translation) {
  position = position + translation;
}

void Transform::Rotate(const float angle, const glm::vec3 axis) {
  rotation = rotation + axis * angle;
}

glm::vec3 Transform::GetForward() {
  UpdateVectors();
  return forward_;
}

glm::vec3 Transform::GetUp() {
  UpdateVectors();
  return up_;
}

glm::vec3 Transform::GetRight() {
  UpdateVectors();
  return right_;
}

void Transform::UpdateVectors() {
  const glm::vec3 new_forward{
      cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y)),
      sin(glm::radians(rotation.x)),
      cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y))};

  forward_ = glm::normalize(new_forward);
  right_ = glm::normalize(glm::cross(forward_, kWorldUp));
  up_ = glm::normalize(glm::cross(right_, forward_));
}

glm::mat4 Transform::GetModelMatrix() const {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

  transform = glm::scale(transform, scale);

  transform = glm::rotate(transform, glm::radians(rotation.x),
                          glm::vec3(1.0f, 0.0f, 0.0f)) *
              glm::rotate(transform, glm::radians(rotation.y),
                          glm::vec3(0.0f, 1.0f, 0.0f)) *
              glm::rotate(transform, glm::radians(rotation.z),
                          glm::vec3(0.0f, 0.0f, 1.0f));

  return transform;
}