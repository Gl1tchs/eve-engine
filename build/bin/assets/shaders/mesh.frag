// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) in vec4 v_albedo;
layout(location = 1) in vec2 v_tex_coords;
layout(location = 2) in float v_diffuse_index;
layout(location = 3) in float v_specular_index;
layout(location = 4) in float v_normal_index;
layout(location = 5) in float v_height_index;

// here `vec3 fragment(vec3 color_in)` will be defined
#pragma custom

layout(location = 0) out vec4 o_color;

uniform sampler2D u_textures[32];

void main() {
  int index = int(v_diffuse_index);
  vec4 color = texture(u_textures[index], v_tex_coords);

#ifdef CUSTOM_SHADER
  color = fragment(color);
#endif

  if (color.a < 0.05) {
    discard;
  }

  o_color = color;
}