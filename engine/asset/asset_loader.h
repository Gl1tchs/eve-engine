// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>

#include "asset/asset.h"
#include "core/utils/memory.h"
#include "graphics/texture.h"

struct AssetLoader {
  static AssetRef<Texture> LoadTexture(const std::filesystem::path& path);
};
