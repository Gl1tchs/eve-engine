// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "graphics/texture.h"
#include "scene/components.h"
#include "scene/model.h"

namespace eve {

namespace asset_loader {

/**
 * @brief Load texture asset from a path.
 * 
 * @param path Relative path of the asset.
 * @return Ref<Texture> Newly created asset.
 */
Ref<Texture> LoadTexture(const fs::path& path);

/**
 * @brief Load model asset from a path.
 * 
 * @param path Relative path of the asset.
 * @return Ref<Model> Newly created asset.
 */
Ref<Model> LoadModel(const fs::path& path);

/**
 * @brief Load a shader instance asset from a path.
 * 
 * @param path Relative path of the asset.
 * @return Ref<ShaderInstance> Newly created asset.
 */
Ref<ShaderInstance> LoadShader(const fs::path& path);

}  // namespace asset_loader

}  // namespace eve