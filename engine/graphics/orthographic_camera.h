// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/camera.h"

class OrthographicCamera : public Camera {
 public:
  OrthographicCamera(float zoom_level = 1.0f, float near_clip = -1.0f,
                     float far_clip = 1.0f);

  [[nodiscard]] glm::mat4 GetProjectionMatrix() const override;

  [[nodiscard]] glm::mat4 GetViewMatrix(Transform& transform) override;

  [[nodiscard]] const float& GetZoomLevel() const { return zoom_level_; }
  void SetZoomLevel(float zoom_level) { zoom_level_ = zoom_level; }

  [[nodiscard]] const float& GetNearClip() const { return near_clip_; }
  void SetNearClip(float near_clip) { near_clip_ = near_clip; }

  [[nodiscard]] const float& GetFarClip() const { return far_clip_; }
  void SetFarClip(float far_clip) { far_clip_ = far_clip; }

 private:
  float zoom_level_;
  float near_clip_;
  float far_clip_;
};
