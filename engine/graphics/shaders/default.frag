// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) out vec4 o_color;
layout(location = 4) in vec4 v_color;
layout(location = 5) in vec2 v_tex_coords;
layout(location = 6) in float v_tex_index;

layout(set = 0, binding = 0) uniform sampler2D u_textures[32];

void main() {
  int index = int(v_tex_index);
  vec4 sampled = texture(u_textures[index], v_tex_coords);
  o_color = sampled * v_color;
}
