// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) in vec4 COLOR;
layout(location = 1) in vec2 TEX_COORDS;
layout(location = 2) in float TEX_INDEX;
layout(location = 3) in vec2 TEX_TILING;
layout(location = 4) in vec2 TEX_OFFSET;

layout(location = 0) out vec4 o_color;

uniform sampler2D u_textures[32];

void main() {
  int index = int(TEX_INDEX);

  vec2 tex_coords = (TEX_COORDS + TEX_OFFSET) * TEX_TILING;
  vec4 texture = texture(u_textures[index], tex_coords);

  o_color = texture * COLOR;
}
