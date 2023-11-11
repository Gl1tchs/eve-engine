// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/camera.h"

class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(float fov = 45.0f, float near_clip = 0.1f,
                    float far_clip = 100.0f);

  [[nodiscard]] glm::mat4 GetViewMatrix(Transform& transform) override;

  [[nodiscard]] glm::mat4 GetProjectionMatrix() const override;

  [[nodiscard]] const float& GetFov() const { return fov_; }
  void SetFov(float fov) { fov_ = fov; }

  [[nodiscard]] const float& GetNearClip() const { return near_clip_; }
  void SetNearClip(float near_clip) { near_clip_ = near_clip; }

  [[nodiscard]] const float& GetFarClip() const { return far_clip_; }
  void SetFarClip(float far_clip) { far_clip_ = far_clip; }

 private:
  float fov_;
  float near_clip_;
  float far_clip_;
};
