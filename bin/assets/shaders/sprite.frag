// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) in vec4 v_color;
layout(location = 1) in vec2 v_tex_coords;
layout(location = 2) in float v_tex_index;
layout(location = 3) in vec2 v_tex_tiling;

layout(location = 0) out vec4 o_color;

uniform sampler2D u_textures[32];

void main() {
  int index = int(v_tex_index);

  vec4 texture = texture(u_textures[index], v_tex_coords * v_tex_tiling);

  vec4 color = texture * v_color;

  if (color.a < 0.05) {
    discard;
  }

  o_color = color;
}
