// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/shader.h"

namespace eve {
[[nodiscard]] std::string SerializeShaderType(ShaderType type);

[[nodiscard]] ShaderType DeserializeShaderType(const std::string& value);

class OpenGLShader final : public Shader {
 public:
  OpenGLShader(const std::string& vs_path, const std::string& fs_path,
               const std::string& gs_path = "");
  ~OpenGLShader();

  void Bind() const override;

  void Unbind() const override;

  void SetUniform(const std::string& name, int value) const override;
  void SetUniform(const std::string& name, float value) const override;
  void SetUniform(const std::string& name, glm::vec2 value) const override;
  void SetUniform(const std::string& name, glm::vec3 value) const override;
  void SetUniform(const std::string& name, glm::vec4 value) const override;
  void SetUniform(const std::string& name,
                  const glm::mat4& value) const override;
  void SetUniform(const std::string& name, int count,
                  int* value) const override;
  void SetUniform(const std::string& name, int count,
                  float* value) const override;

  [[nodiscard]] uint32_t GetProgramID() const { return program_; }

 private:
  [[nodiscard]] static std::string LoadShaderSource(
      const std::filesystem::path& path);

  [[nodiscard]] int GetUniformLocation(const std::string& name) const;

  [[nodiscard]] static bool CheckCompileErrors(uint32_t shader,
                                               ShaderType type);

  [[nodiscard]] static uint32_t CompileShader(const std::string& source,
                                              ShaderType type);

 private:
  uint32_t program_;
};
}  // namespace eve