// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_loader.h"

namespace eve {

Ref<Texture> AssetLoader::LoadTexture(const fs::path& path) {
  Ref<Texture> texture = Texture::Create(path);
  return texture;
}

Ref<Model> AssetLoader::LoadModel(const fs::path& path) {
  Ref<Model> model = Model::Create(path);
  return model;
}

Ref<ShaderInstance> AssetLoader::LoadShader(const fs::path& path) {
  Ref<ShaderInstance> shader = CreateRef<ShaderInstance>();
  return shader;
}

}  // namespace eve