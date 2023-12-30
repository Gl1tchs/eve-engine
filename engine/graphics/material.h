// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/texture.h"

namespace eve {
struct Material {
  glm::vec3 albedo;
  float metallic;
  float roughness;
  float ao;
};
}  // namespace eve