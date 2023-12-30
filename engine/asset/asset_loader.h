// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/texture.h"
#include "scene/components.h"
#include "scene/static_mesh.h"

namespace eve {

struct AssetLoader {
  static AssetRef<Texture> LoadTexture(const std::string& path);

  static AssetRef<Model> LoadModel(const std::string& path);
};

}  // namespace eve