// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>
#include "pch_shared.h"

#include "core/utils/guuid.h"
#include "graphics/material.h"
#include "graphics/orthographic_camera.h"
#include "graphics/perspective_camera.h"
#include "graphics/texture.h"
#include "scene/lights.h"
#include "scene/static_mesh.h"
#include "scripting/script.h"

struct IdComponent {
  GUUID id;
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

struct ScriptComponent {
  std::string path;
  Ref<Script> instance;
};

template <typename... Component>
struct ComponentGroup {};

using AllComponents =
    ComponentGroup<Transform, CameraComponent, ModelComponent, Material,
                   DirectionalLight, ScriptComponent>;