// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/texture.h"
#include "scene/components.h"
#include "scene/model.h"

namespace eve {

namespace asset_loader {

Ref<Texture> LoadTexture(const fs::path& path);

Ref<Model> LoadModel(const fs::path& path);

Ref<ShaderInstance> LoadShader(const fs::path& path);

}  // namespace asset_loader

}  // namespace eve