// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef MATERIAL_GLSL_
#define MATERIAL_GLSL_

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

#endif // MATERIAL_GLSL_