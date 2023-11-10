// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(float zoom_level, float near_clip,
                                       float far_clip)
    : zoom_level_(zoom_level), near_clip_(near_clip), far_clip_(far_clip) {}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const {
  return glm::ortho(-GetAspectRatio() * zoom_level_,
                    GetAspectRatio() * zoom_level_, -zoom_level_, zoom_level_,
                    near_clip_, far_clip_);
}

glm::mat4 OrthographicCamera::GetViewMatrix(Transform& transform) {
  return glm::inverse(transform.GetModelMatrix());
}