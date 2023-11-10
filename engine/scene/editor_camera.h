// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/math/transform.h"
#include "graphics/orthographic_camera.h"

class EditorCamera : public OrthographicCamera {
 public:
  EditorCamera();

  [[nodiscard]] glm::mat4 GetViewMatrix();

  [[nodiscard]] Transform& GetTransform() { return transform_; }
  [[nodiscard]] const Transform& GetTransform() const { return transform_; }

  void Update(float ds);

 private:
  Transform transform_;
};
