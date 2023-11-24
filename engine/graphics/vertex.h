// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/material.h"

struct Vertex final {
  glm::vec4 position;
  glm::vec3 normal;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  glm::vec2 tex_coords;
  float tex_index = 0.0f;
  int entity_id = -1;
};

struct RenderPacket final {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  Material material{};
};
