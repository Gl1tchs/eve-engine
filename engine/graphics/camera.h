// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/math/transform.h"

class Camera {
 public:
  virtual ~Camera() = default;

  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;

  [[nodiscard]] virtual glm::mat4 GetViewMatrix(Transform& transform) = 0;

  [[nodiscard]] const float& GetAspectRatio() const { return aspect_ratio_; }
  void SetAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; };

 private:
  float aspect_ratio_;
};
