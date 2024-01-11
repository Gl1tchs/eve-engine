// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/transform.h"

namespace eve {

struct Camera {
  float aspect_ratio;

  virtual ~Camera() = default;

  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;

  [[nodiscard]] virtual glm::mat4 GetViewMatrix(const Transform& transform) = 0;
};

}  // namespace eve