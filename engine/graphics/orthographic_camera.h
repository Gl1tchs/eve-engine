// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/camera.h"

namespace eve {
struct OrthographicCamera : Camera {
  float zoom_level = 1.0f;
  float near_clip = -1.0f;
  float far_clip = 1.0f;

  virtual ~OrthographicCamera() = default;

  [[nodiscard]] glm::mat4 GetProjectionMatrix() const override;

  [[nodiscard]] glm::mat4 GetViewMatrix(Transform& transform) override;
};
}  // namespace eve