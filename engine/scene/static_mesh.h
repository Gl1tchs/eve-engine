// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <vector>

#include "core/utils/memory.h"
#include "graphics/material.h"
#include "graphics/vertex.h"

struct StaticMesh {
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  Material material;
};

struct Model {
  std::vector<StaticMesh> meshes;

  static Ref<Model> Load(const std::filesystem::path& path);
};

struct ModelComponent {
  AssetRef<Model> model;
};
