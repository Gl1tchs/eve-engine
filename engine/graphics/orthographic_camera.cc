// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace eve {
glm::mat4 OrthographicCamera::GetProjectionMatrix() const {
  return glm::ortho(-aspect_ratio * zoom_level, aspect_ratio * zoom_level,
                    -zoom_level, zoom_level, near_clip, far_clip);
}

glm::mat4 OrthographicCamera::GetViewMatrix(Transform& transform) {
  return glm::inverse(transform.GetModelMatrix());
}
}  // namespace eve