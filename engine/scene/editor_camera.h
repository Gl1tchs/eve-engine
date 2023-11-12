// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "scene/transform.h"
#include "graphics/perspective_camera.h"

class EditorCamera : public PerspectiveCamera {
 public:
  EditorCamera();

  [[nodiscard]] glm::mat4 GetViewMatrix();

  [[nodiscard]] Transform& GetTransform() { return transform_; }
  [[nodiscard]] const Transform& GetTransform() const { return transform_; }

  void Update(float ds);

 private:
  Transform transform_;
};
