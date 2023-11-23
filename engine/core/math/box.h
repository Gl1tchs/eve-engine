// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

struct Box {
  glm::vec3 bottom_left;
  glm::vec3 bottom_right;
  glm::vec3 top_left;
  glm::vec3 top_right;
};
