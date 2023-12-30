// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/camera.h"

namespace eve {
struct PerspectiveCamera : Camera {
  float fov = 45.0f;
  float near_clip = 0.1f;
  float far_clip = 300.0f;

  virtual ~PerspectiveCamera() = default;

  [[nodiscard]] glm::mat4 GetViewMatrix(Transform& transform) override;

  [[nodiscard]] glm::mat4 GetProjectionMatrix() const override;
};
}  // namespace eve