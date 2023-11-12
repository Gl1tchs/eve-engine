// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/editor_camera.h"

#include "core/event/input.h"

EditorCamera::EditorCamera() : PerspectiveCamera() {
  transform_.position = {0, 0, -5};
}

glm::mat4 EditorCamera::GetViewMatrix() {
  return PerspectiveCamera::GetViewMatrix(transform_);
}

void EditorCamera::Update(float ds) {}
