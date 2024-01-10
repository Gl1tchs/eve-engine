// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "camera_data.glsl"

layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec2 a_tex_coords;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_tex_index;
layout(location = 4) in vec2 a_tex_tiling;
layout(location = 5) in vec2 a_tex_offset;


layout(location = 0) out vec4 COLOR;
layout(location = 1) out vec2 TEX_COORDS;
layout(location = 2) out float TEX_INDEX;
layout(location = 3) out vec2 TEX_TILING;
layout(location = 4) out vec2 TEX_OFFSET;

void main() {
  COLOR = a_color;
  TEX_COORDS = a_tex_coords;
  TEX_INDEX = a_tex_index;
  TEX_TILING = a_tex_tiling;
  TEX_OFFSET = a_tex_offset;

  gl_Position = CAMERA.proj * CAMERA.view * a_pos;
}
