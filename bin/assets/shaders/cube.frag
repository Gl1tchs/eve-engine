// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) in vec4 v_color;
layout(location = 1) in vec2 v_tex_coords;

layout(location = 0) out vec4 o_color;

void main() {
  vec4 color = v_color;

  if (color.a < 0.05) {
    discard;
  }

  o_color = color;
}
