// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) in vec3 v_tex_coord;

layout(location = 0) out vec4 o_color;

uniform samplerCube u_skybox;

void main() {
  o_color = texture(u_skybox, v_tex_coord);
}
