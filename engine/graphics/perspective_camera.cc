#include "graphics/perspective_camera.h"

namespace eve {

glm::mat4 PerspectiveCamera::GetViewMatrix(const Transform& transform) {
  return glm::lookAt(transform.GetPosition(),
                     transform.GetPosition() + transform.GetForward(),
                     transform.GetUp());
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
  return glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}

}  // namespace eve