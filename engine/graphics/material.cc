// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/material.h"
#include "material.h"

namespace eve {

ShaderUniformType ConvertStringToShaderUniformType(
    const std::string_view& type_str) {
  if (type_str == "float") {
    return ShaderUniformType::kFloat;
  } else if (type_str == "vec2") {
    return ShaderUniformType::kFloat2;
  } else if (type_str == "vec3") {
    return ShaderUniformType::kFloat3;
  } else if (type_str == "vec4") {
    return ShaderUniformType::kFloat4;
  } else if (type_str == "mat3") {
    return ShaderUniformType::kMat3;
  } else if (type_str == "mat4") {
    return ShaderUniformType::kMat4;
  } else if (type_str == "int") {
    return ShaderUniformType::kInt;
  } else if (type_str == "sampler2D") {
    return ShaderUniformType::kTexture2D;
  } else if (type_str == "bool") {
    return ShaderUniformType::kBool;
  } else {
    return ShaderUniformType::kNone;
  }
}

std::string ConvertUniformTypeToString(const ShaderUniformType& type) {
  switch (type) {
    case ShaderUniformType::kFloat: {
      return "float";
    }
    case ShaderUniformType::kFloat2: {
      return "vec2";
    }
    case ShaderUniformType::kFloat3: {
      return "vec3";
    }
    case ShaderUniformType::kFloat4: {
      return "vec4";
    }
    case ShaderUniformType::kMat3: {
      return "mat3";
    }
    case ShaderUniformType::kMat4: {
      return "mat4";
    }
    case ShaderUniformType::kInt: {
      return "int";
    }
    case ShaderUniformType::kTexture2D: {
      return "sampler2D";
    }
    case ShaderUniformType::kBool: {
      return "bool";
    }
    default: {
      return "";
    }
  }
}

int GetUniformSize(const ShaderUniformType& uniform_type) {
  switch (uniform_type) {
    case ShaderUniformType::kFloat: {
      return sizeof(float);
    }
    case ShaderUniformType::kFloat2: {
      return 2 * sizeof(float);
    }
    case ShaderUniformType::kFloat3: {
      return 3 * sizeof(float);
    }
    case ShaderUniformType::kFloat4: {
      return 4 * sizeof(float);
    }
    case ShaderUniformType::kMat3: {
      return 9 * sizeof(float);
    }
    case ShaderUniformType::kMat4: {
      return 16 * sizeof(float);
    }
    case ShaderUniformType::kInt:
    case ShaderUniformType::kTexture2D: {
      return sizeof(int);
    }
    case ShaderUniformType::kBool: {
      return sizeof(bool);
    }
    default: {
      return 0;
    }
  }
}

ShaderValueVariant GetDefaultShaderValue(const ShaderUniformType& type) {
  switch (type) {
    case ShaderUniformType::kFloat: {
      return 0.0f;
    }
    case ShaderUniformType::kFloat2: {
      return glm::vec2(0.0f);
    }
    case ShaderUniformType::kFloat3: {
      return glm::vec3(0.0f);
    }
    case ShaderUniformType::kFloat4: {
      return glm::vec4(0.0f);
    }
    case ShaderUniformType::kMat3: {
      return glm::mat3(1.0f);
    }
    case ShaderUniformType::kMat4: {
      return glm::mat4(1.0f);
    }
    case ShaderUniformType::kInt:
    case ShaderUniformType::kTexture2D: {
      return 0;
    }
    case ShaderUniformType::kBool: {
      return false;
    }
    default: {
      return 0;
    }
  }
}

}  // namespace eve
