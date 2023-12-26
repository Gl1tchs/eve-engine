// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/primitives/mesh.h"

struct Model {
  std::vector<RenderData<MeshVertex>> meshes;

  static Ref<Model> Load(const std::filesystem::path& path);
};

struct ModelComponent {
  AssetRef<Model> model;
};
