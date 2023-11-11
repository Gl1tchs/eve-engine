// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>

#include "asset/asset_library.h"
#include "core/utils/guuid.h"
#include "graphics/orthographic_camera.h"
#include "graphics/perspective_camera.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"

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

struct DrawableComponent {
  RenderPacket packet;
  AssetRef<Texture> texture;
};

template <typename... Component>
struct ComponentGroup {};

using AllComponents =
    ComponentGroup<Transform, CameraComponent, DrawableComponent>;