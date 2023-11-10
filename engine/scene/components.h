// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>

#include "core/utils/guuid.h"
#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"

struct IDComponent {
  GUUID id;
};

struct TagComponent {
  std::string tag;
};

struct CameraComponent {
  Camera* camera;

  bool primary = true;
  bool fixed_aspect_ratio = false;
};

struct DrawableComponent {
  RenderPacket packet;
  Ref<Texture> texture;
};
