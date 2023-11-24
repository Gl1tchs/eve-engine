// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "default_output.glsl"
#include "directional_light.glsl"
#include "camera_data.glsl"

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entity_id;

layout(location = 0) in DefaultVertexOutput v_input;
layout(location = 7) in flat float v_tex_index;
layout(location = 8) in flat int v_entity_id;

layout(set = 0, binding = 0) uniform sampler2D u_textures[32];

layout (std140, binding = 1) uniform LightData {
  DirectionalLight directional;
} u_light_data;

void main() {
  vec3 norm = normalize(v_input.normal);
  vec3 view_dir = normalize(u_camera.position - v_input.frag_pos);

  // int index = int(v_tex_index);
  // vec4 sampled = texture(u_textures[index], v_input.tex_coords);

  vec3 result = CalculateDirectionalLight(u_light_data.directional, v_input.material, norm, view_dir);

  o_color = vec4(result, 1.0);
  o_entity_id = v_entity_id;
}
