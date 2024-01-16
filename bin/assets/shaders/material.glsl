// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef MATERIAL_GLSL_
#define MATERIAL_GLSL_

struct Material {
  vec3 albedo;
  float metallic;
  float roughness;
  float ao;
};

#endif  // MATERIAL_GLSL_