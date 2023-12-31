// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/material.h"

enum class ShaderType { kNone = 0, kVertex, kFragment, kGeometry };

namespace eve {
class Shader {
 public:
  virtual void Recompile(const std::string& vs_path, const std::string& fs_path,
                         const std::string& custom_shader = "") = 0;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetUniform(const std::string& name,
                          ShaderValueVariant value) const = 0;
  virtual void SetUniform(const std::string& name, int value) const = 0;
  virtual void SetUniform(const std::string& name, float value) const = 0;
  virtual void SetUniform(const std::string& name, glm::vec2 value) const = 0;
  virtual void SetUniform(const std::string& name, glm::vec3 value) const = 0;
  virtual void SetUniform(const std::string& name, glm::vec4 value) const = 0;
  virtual void SetUniform(const std::string& name,
                          const glm::mat3& value) const = 0;
  virtual void SetUniform(const std::string& name,
                          const glm::mat4& value) const = 0;
  virtual void SetUniform(const std::string& name, int count,
                          int* value) const = 0;
  virtual void SetUniform(const std::string& name, int count,
                          float* value) const = 0;

  [[nodiscard]] virtual const std::vector<ShaderUniform>& GetUniformFields()
      const = 0;

  [[nodiscard]] static Ref<Shader> Create(
      const std::string& vs_path, const std::string& fs_path,
      const std::string& custom_shader = "");
};
}  // namespace eve