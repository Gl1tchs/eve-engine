// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/math/transform.h"

class Camera {
 public:
  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;

  [[nodiscard]] virtual glm::mat4 GetViewMatrix(Transform& transform) = 0;
};
