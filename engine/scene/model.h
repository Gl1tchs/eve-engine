// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/primitives/mesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;

namespace eve {

struct Model : Asset {
  EVE_IMPL_ASSET(AssetType::kStaticMesh)

  std::vector<RenderData<MeshVertex>> meshes;
  std::vector<AssetHandle> textures;

  static Ref<Model> Create(const fs::path& path);

 private:
  void ProcessNode(aiNode* node, const aiScene* scene);

  RenderData<MeshVertex> ProcessMesh(aiMesh* mesh, const aiScene* scene);

 private:
  fs::path directory_;
};

struct ModelComponent {
  AssetHandle model = 0;
};

}  // namespace eve
