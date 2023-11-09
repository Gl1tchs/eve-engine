// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(float aspect_ratio, float zoom_level,
                                       float near_clip, float far_clip)
    : aspect_ratio_(aspect_ratio),
      zoom_level_(zoom_level),
      near_clip_(near_clip),
      far_clip_(far_clip) {}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const {
  return glm::ortho(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_,
                    -zoom_level_, zoom_level_, near_clip_, far_clip_);
}

glm::mat4 OrthographicCamera::GetViewMatrix(Transform& transform) {
  return glm::inverse(transform.GetModelMatrix());
}