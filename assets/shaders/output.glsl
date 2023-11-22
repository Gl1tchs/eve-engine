// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef OUTPUT_GLSL_
#define OUTPUT_GLSL_

#include "material.glsl"

struct VertexOutput {
  Material material;
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  float tex_index;
};

#endif // OUTPUT_GLSL_