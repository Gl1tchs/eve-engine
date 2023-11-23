// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "line_output.glsl"
#include "camera_data.glsl"

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

layout(location = 0) out LineVertexOutput v_output;

void main() {
  v_output.color = a_color;
  
  gl_Position = u_camera.proj * u_camera.view * vec4(a_pos, 1.0);
}
