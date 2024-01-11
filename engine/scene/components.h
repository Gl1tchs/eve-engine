// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"
#include "graphics/material.h"
#include "graphics/orthographic_camera.h"
#include "graphics/perspective_camera.h"
#include "physics/box_collider.h"
#include "physics/rigidbody.h"
#include "scene/model.h"

namespace eve {
struct IdComponent {
  UUID id;
};

struct TagComponent {
  std::string tag;
};

struct CameraComponent {
  OrthographicCamera ortho_camera;
  PerspectiveCamera persp_camera;
  bool is_orthographic;
  // TODO put this in scene
  bool is_primary = true;
  bool is_fixed_aspect_ratio = false;

  inline void SetAspectRatio(const float& ratio) {
    if (is_fixed_aspect_ratio) {
      return;
    }

    persp_camera.aspect_ratio = ratio;
    ortho_camera.aspect_ratio = ratio;
  }

  [[nodiscard]] inline glm::mat4 GetProjectionMatrix() const {
    if (is_orthographic) {
      return ortho_camera.GetProjectionMatrix();
    } else {
      return persp_camera.GetProjectionMatrix();
    }
  }

  [[nodiscard]] inline glm::mat4 GetViewMatrix(const Transform& transform) {
    if (is_orthographic) {
      return ortho_camera.GetViewMatrix(transform);
    } else {
      return persp_camera.GetViewMatrix(transform);
    }
  }
};

struct SpriteRendererComponent {
  AssetHandle texture = 0;
  Color color = kColorWhite;
  glm::vec2 tex_tiling = {1, 1};
  glm::vec2 tex_offset = {0, 0};
};

class Script;

struct ScriptComponent {
  std::string class_name;
};

template <typename... Component>
struct ComponentGroup {};

// All component types must be registered here in order to use them in scripts and copy between scenes.
using AllComponents =
    ComponentGroup<Transform, CameraComponent, SpriteRendererComponent,
                   ModelComponent, Material, Rigidbody, BoxCollider,
                   ScriptComponent>;
}  // namespace eve
