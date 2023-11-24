// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef DEFAULT_OUTPUT_GLSL_
#define DEFAULT_OUTPUT_GLSL_

#include "material.glsl"

struct DefaultVertexOutput {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  Material material;
};

#endif // DEFAULT_OUTPUT_GLSL_
