// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "default_output.glsl"
#include "camera_data.glsl"

layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec3 a_ambient;
layout(location = 2) in vec3 a_diffuse;
layout(location = 3) in vec3 a_specular;
layout(location = 4) in float a_shininess;
layout(location = 5) in vec3 a_normal;
layout(location = 6) in vec2 a_tex_coords;
layout(location = 7) in float a_tex_index;

layout(location = 0) out DefaultVertexOutput v_output;

void main() {
  // assign material values
  Material material;
  material.ambient = a_ambient;
  material.diffuse = a_diffuse;
  material.specular = a_specular;
  material.shininess = a_shininess;

  v_output.material = material;
  v_output.frag_pos = a_pos.xyz;
  v_output.normal = a_normal;
  v_output.tex_coords = a_tex_coords;
  v_output.tex_index = a_tex_index;
  
  gl_Position = u_camera.proj * u_camera.view * a_pos;
}
