// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/primitives/mesh.h"

namespace eve {
struct Model : Asset {
  IMPL_ASSET(AssetType::kStaticMesh)

  std::vector<RenderData<MeshVertex>> meshes;

  static Ref<Model> Create(const fs::path& path);
};

struct ModelComponent {
  UUID model;
};
}  // namespace eve
