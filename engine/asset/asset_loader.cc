// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_loader.h"

namespace eve {

namespace asset_loader {

Ref<Texture> LoadTexture(const fs::path& path) {
  if (!fs::exists(path)) {
    return nullptr;
  }

  Ref<Texture> texture = Texture::Create(path);
  return texture;
}

Ref<Model> LoadModel(const fs::path& path) {
  if (!fs::exists(path)) {
    return nullptr;
  }

  Ref<Model> model = Model::Create(path);
  return model;
}

Ref<ShaderInstance> LoadShader(const fs::path& path) {
  if (!fs::exists(path)) {
    return nullptr;
  }

  Ref<ShaderInstance> shader = CreateRef<ShaderInstance>();
  return shader;
}

}  // namespace asset_loader

}  // namespace eve