// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>

#include "asset/asset.h"
#include "core/utils/memory.h"
#include "graphics/texture.h"
#include "scene/components.h"
#include "scene/static_mesh.h"

struct AssetLoader {
  static AssetRef<Texture> LoadTexture(const std::string& path);

  static AssetRef<Model> LoadModel(const std::string& path);
};
