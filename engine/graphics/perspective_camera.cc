#include "graphics/perspective_camera.h"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float near_clip, float far_clip)
    : fov_(fov), near_clip_(near_clip), far_clip_(far_clip) {}

glm::mat4 PerspectiveCamera::GetViewMatrix(Transform& transform) {
  return glm::lookAt(transform.position,
                     transform.position + transform.GetForward(),
                     transform.GetUp());
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
  return glm::perspective(glm::radians(fov_), GetAspectRatio(), near_clip_,
                          far_clip_);
}