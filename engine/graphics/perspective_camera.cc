#include "graphics/perspective_camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 PerspectiveCamera::GetViewMatrix(Transform& transform) {
  return glm::lookAt(transform.position,
                     transform.position + transform.GetForward(),
                     transform.GetUp());
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
  return glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}