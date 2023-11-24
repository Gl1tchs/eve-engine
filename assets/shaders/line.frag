// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entity_id;

layout(location = 0) in vec4 v_color;
layout(location = 1) in flat int v_entity_id;

void main() {
  o_color = v_color;
  o_entity_id = v_entity_id;
}
