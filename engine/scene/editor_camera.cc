// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/editor_camera.h"

#include "core/event/input.h"

EditorCamera::EditorCamera() : PerspectiveCamera() {
  ResetTransform();

  last_mouse_pos_ = Input::GetMousePosition();
}

glm::mat4 EditorCamera::GetViewMatrix() {
  return PerspectiveCamera::GetViewMatrix(transform_);
}

void EditorCamera::Update(float ds) {
  glm::vec2 mouse_delta = Input::GetMousePosition() - last_mouse_pos_;

  glm::vec3 new_rotation =
      transform_.rotation +
      glm::vec3(-mouse_delta.y, -mouse_delta.x, 0.0f) * sensitivity_;

  // clamp between (-90,90) to make it realistic :)
  new_rotation.x = glm::clamp(new_rotation.x, -89.0f, 89.0f);

  transform_.rotation = new_rotation;

  // store last mouse pos to prevent instant rotations
  last_mouse_pos_ = Input::GetMousePosition();

  // forward / backward controls
  if (Input::IsKeyPressed(KeyCode::kW)) {
    transform_.position += transform_.GetForward() * speed_ * ds;
  }
  if (Input::IsKeyPressed(KeyCode::kS)) {
    transform_.position -= transform_.GetForward() * speed_ * ds;
  }
  
  // right / left controls
  if (Input::IsKeyPressed(KeyCode::kD)) {
    transform_.position += transform_.GetRight() * speed_ * ds;
  }
  if (Input::IsKeyPressed(KeyCode::kA)) {
    transform_.position -= transform_.GetRight() * speed_ * ds;
  }
  
  // up / down controls
  if (Input::IsKeyPressed(KeyCode::kE)) {
    transform_.position = kWorldUp * speed_ * ds;
  }
  if (Input::IsKeyPressed(KeyCode::kQ)) {
    transform_.position -= kWorldUp * speed_ * ds;
  }
}

void EditorCamera::ResetTransform() {
  transform_.position = {0, 0, 5};
  transform_.rotation = {0, 0, 0};
  transform_.scale = {1, 1, 1};
}

void EditorCamera::ResetMousePos() {
  last_mouse_pos_ = Input::GetMousePosition();
}
