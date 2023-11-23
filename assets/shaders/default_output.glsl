// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef DEFAULT_OUTPUT_GLSL_
#define DEFAULT_OUTPUT_GLSL_

#include "material.glsl"

struct DefaultVertexOutput {
  Material material;
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  float tex_index;
};

#endif // DEFAULT_OUTPUT_GLSL_
