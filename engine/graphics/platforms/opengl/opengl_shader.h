// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/shader.h"

#include "graphics/material.h"

namespace eve {

[[nodiscard]] std::string SerializeShaderType(ShaderType type);

[[nodiscard]] ShaderType DeserializeShaderType(const std::string& value);

class OpenGLShader final : public Shader {
 public:
  OpenGLShader(const std::string& vs_path, const std::string& fs_path,
               const std::string& custom_shader = "");
  ~OpenGLShader();

  void Recompile(const std::string& vs_path, const std::string& fs_path,
                 const std::string& custom_shader = "") override;

  void Bind() const override;

  void Unbind() const override;

  void SetUniform(const std::string& name,
                  ShaderValueVariant value) const override;
  void SetUniform(const std::string& name, int value) const override;
  void SetUniform(const std::string& name, float value) const override;
  void SetUniform(const std::string& name, glm::vec2 value) const override;
  void SetUniform(const std::string& name, glm::vec3 value) const override;
  void SetUniform(const std::string& name, glm::vec4 value) const override;
  void SetUniform(const std::string& name,
                  const glm::mat3& value) const override;
  void SetUniform(const std::string& name,
                  const glm::mat4& value) const override;
  void SetUniform(const std::string& name, int count,
                  int* value) const override;
  void SetUniform(const std::string& name, int count,
                  float* value) const override;

  [[nodiscard]] const std::vector<ShaderUniform>& GetUniformFields() const {
    return custom_uniforms_;
  };

  [[nodiscard]] uint32_t GetProgramID() const { return program_; }

 private:
  [[nodiscard]] std::string LoadShaderSource(
      const std::filesystem::path& path, bool is_fragment = false,
      const std::string& custom_shader = "");

  [[nodiscard]] bool ParseCustomShader(const std::string& custom_shader);

  [[nodiscard]] int GetUniformLocation(const std::string& name) const;

  [[nodiscard]] static bool CheckCompileErrors(uint32_t shader,
                                               ShaderType type);

  [[nodiscard]] static uint32_t CompileShader(const std::string& source,
                                              ShaderType type);

 private:
  uint32_t program_;

  std::vector<ShaderUniform> custom_uniforms_;
};
}  // namespace eve