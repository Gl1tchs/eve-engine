// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/perspective_camera.h"
#include "scene/transform.h"

namespace eve {
class EditorCamera : public PerspectiveCamera {
 public:
  EditorCamera();

  [[nodiscard]] glm::mat4 GetViewMatrix();

  [[nodiscard]] Transform& GetTransform() { return transform_; }
  [[nodiscard]] const Transform& GetTransform() const { return transform_; }

  void Update(float ds);

  void SetSpeed(float value) { speed_ = value; }
  [[nodiscard]] float GetSpeed() const { return speed_; }

  void SetSensitivity(float value) { sensitivity_ = value; }
  [[nodiscard]] float GetSensitivity() const { return sensitivity_; }

  void ResetTransform();

  void ResetMousePos();

 private:
  Transform transform_;

  float speed_{5.0f};
  float sensitivity_{0.1f};

  glm::dvec2 last_mouse_pos_;
};
}  // namespace eve
