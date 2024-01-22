// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "camera_data.glsl"
#include "material.glsl"

layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec4 a_albedo;
layout(location = 2) in float a_metallic;
layout(location = 3) in float a_roughness;
layout(location = 4) in float a_ao;
layout(location = 5) in vec3 a_normal;
layout(location = 6) in vec2 a_tex_coords;
layout(location = 7) in float a_tex_index;
layout(location = 8) in mat3 a_normal_matrix;

layout(location = 0) out vec3 FRAG_POS;
layout(location = 1) out vec3 NORMAL;
layout(location = 2) out vec2 TEX_COORDS;
layout(location = 3) out float TEX_INDEX;
layout(location = 4) out Material MATERIAL;

void main() {
  // assign material values
  Material material;
  material.albedo = a_albedo.rgb;
  material.metallic = a_metallic;
  material.roughness = a_roughness;
  material.ao = a_ao;

  FRAG_POS = a_pos.xyz;
  NORMAL = a_normal_matrix * a_normal;
  TEX_COORDS = a_tex_coords;
  TEX_INDEX = a_tex_index;
  MATERIAL = material;

  gl_Position = u_camera.proj * u_camera.view * a_pos;
}
