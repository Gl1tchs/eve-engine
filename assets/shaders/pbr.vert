// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "material.glsl"
#include "camera_data.glsl"

layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec3 a_albedo;
layout(location = 2) in float a_metallic;
layout(location = 3) in float a_roughness;
layout(location = 4) in float a_ao;
layout(location = 5) in vec3 a_normal;
layout(location = 6) in vec2 a_tex_coords;
layout(location = 7) in float a_tex_index;
layout(location = 8) in mat3 a_normal_matrix;

struct VertexOutput {
  Material material;
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  float tex_index;
};

layout(location = 0) out VertexOutput v_output;

void main() {
  // assign material values
  Material material;
  material.albedo = a_albedo;
  material.roughness = a_roughness;
  material.ao = a_ao;

  v_output.material = material;
  v_output.frag_pos = a_pos.xyz;
  v_output.normal = a_normal_matrix * a_normal;
  v_output.tex_coords = a_tex_coords;
  v_output.tex_index = a_tex_index;

  gl_Position = u_camera.proj * u_camera.view * a_pos;
}
