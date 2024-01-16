// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "camera_data.glsl"

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_tex_coords;
layout(location = 2) in vec4 a_color;


layout(location = 0) out vec4 v_color;
layout(location = 1) out vec2 v_tex_coords;

void main() {
  v_color = a_color;
  v_tex_coords = a_tex_coords;

  gl_Position = CAMERA.proj * CAMERA.view * a_position;
}
