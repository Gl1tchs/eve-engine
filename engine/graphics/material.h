// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/texture.h"

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  // glm::vec4 ambient_color;
  // glm::vec4 diffuse_color;
  // glm::vec4 specular_color;
  // float specular_exponent;
  // float optical_density;
  // float disolve_variable;
  // int illumination_variable;
  // AssetRef<Texture> texture;
  // AssetRef<Texture> ambient_map;
  // AssetRef<Texture> diffuse_map;
  // AssetRef<Texture> specular_map;
  // AssetRef<Texture> alpha_map;
  // AssetRef<Texture> bump_map;
};
