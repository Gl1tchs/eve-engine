// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "camera_data.glsl"

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_albedo;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_tex_coords;
layout(location = 4) in float a_diffuse_index;

layout(location = 0) out vec4 v_albedo;
layout(location = 1) out vec2 v_tex_coords;
layout(location = 2) out float v_diffuse_index;

void main() {
  v_albedo = a_albedo;
  v_tex_coords = a_tex_coords;
  v_diffuse_index = a_diffuse_index;

  gl_Position = u_camera.proj * u_camera.view * a_position;
}
