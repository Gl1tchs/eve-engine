// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/editor_camera.h"

#include "core/event/input.h"

EditorCamera::EditorCamera() : OrthographicCamera() {}

glm::mat4 EditorCamera::GetViewMatrix() {
  return OrthographicCamera::GetViewMatrix(transform_);
}

void EditorCamera::Update(float ds) {}
