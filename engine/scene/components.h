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

class Script;

struct ScriptComponent {
  std::string class_name;
};

template <typename... Component>
struct ComponentGroup {};

using AllComponents = ComponentGroup<Transform, CameraComponent, ModelComponent,
                                     Material, ScriptComponent>;
}  // namespace eve
