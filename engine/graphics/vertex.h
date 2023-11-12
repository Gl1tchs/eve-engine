// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

struct Vertex final {
  glm::vec4 position;
  glm::vec4 color;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  float tex_index = 0.0f;
};

struct RenderPacket final {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
};
