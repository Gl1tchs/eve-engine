// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_loader.h"

#include "asset/asset_library.h"

namespace eve {

AssetRef<Texture> AssetLoader::LoadTexture(const std::string& path) {
  auto path_abs = AssetLibrary::GetAssetPath(path);
  if (!fs::exists(path_abs)) {
    return nullptr;
  }

  AssetRef<Texture> texture = CreateRef<Asset<Texture>>();
  texture->id = UUID();
  texture->path = path;
  texture->asset = Texture::Create(path_abs);

  return texture;
}

AssetRef<Model> AssetLoader::LoadModel(const std::string& path) {
  auto path_abs = AssetLibrary::GetAssetPath(path);
  if (!fs::exists(path_abs)) {
    return nullptr;
  }

  AssetRef<Model> model = CreateRef<Asset<Model>>();
  model->id = UUID();
  model->path = path;
  model->asset = Model::Load(path_abs);

  return model;
}

}  // namespace eve