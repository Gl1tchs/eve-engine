// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"
#include "graphics/material.h"
#include "graphics/orthographic_camera.h"
#include "graphics/perspective_camera.h"
#include "scene/static_mesh.h"

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

using AllComponents =
    ComponentGroup<Transform, CameraComponent, SpriteRendererComponent,
                   ModelComponent, Material, ScriptComponent>;
}  // namespace eve
