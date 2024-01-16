// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "asset/asset.h"
#include "core/buffer.h"
#include "core/color.h"
#include "graphics/texture.h"

namespace eve {

enum class ShaderUniformType {
  kNone,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kMat3,
  kMat4,
  kInt,
  kBool,
  kTexture2D
};

typedef std::variant<float, glm::vec2, glm::vec3, glm::vec4, glm::mat3,
                     glm::mat4, int, bool>
    ShaderValueVariant;

struct ShaderUniform {
  std::string name;
  ShaderUniformType type;
  ShaderValueVariant value;
};

ShaderUniformType ConvertStringToShaderUniformType(
    const std::string_view& type_str);

std::string ConvertUniformTypeToString(const ShaderUniformType& type);

int GetUniformSize(const ShaderUniformType& uniform_type);

ShaderValueVariant GetDefaultShaderValue(const ShaderUniformType& type);

struct ShaderInstance : public Asset {
  std::vector<ShaderUniform> uniforms{};

  EVE_IMPL_ASSET(AssetType::kShader)
};

// TODO make this work as an asset
struct Material : public Asset {
  Color albedo = kColorWhite;
  AssetHandle shader = 0;

  EVE_IMPL_ASSET(AssetType::kMaterial)
};

}  // namespace eve