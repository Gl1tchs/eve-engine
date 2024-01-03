// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/texture.h"
#include "scene/components.h"
#include "scene/static_mesh.h"

namespace eve {

struct AssetLoader {
  static Ref<Texture> LoadTexture(const fs::path& path);

  static Ref<Model> LoadModel(const fs::path& path);

  static Ref<ShaderInstance> LoadShader(const fs::path& path);
};

}  // namespace eve