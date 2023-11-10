// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "compiler.h"

#include <fstream>
#include <iostream>

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

std::optional<TargetEnv> DeserializeTargetEnv(std::string env) {
  if (env == "vulkan") {
    return std::optional<TargetEnv>(TargetEnv::kVulkan);
  } else if (env == "opengl") {
    return std::optional<TargetEnv>(TargetEnv::kOpenGL);
  } else {
    return std::optional<TargetEnv>();
  }
}

std::optional<ShaderKind> DeserializeShaderKind(std::string kind) {
  if (kind == "vertex") {
    return std::optional<ShaderKind>(ShaderKind::kVertex);
  } else if (kind == "fragment") {
    return std::optional<ShaderKind>(ShaderKind::kFragment);
  } else {
    return std::optional<ShaderKind>();
  }
}

shaderc_target_env GetTargetEnv(TargetEnv env) {
  switch (env) {
    case TargetEnv::kVulkan:
      return shaderc_target_env_vulkan;
    case TargetEnv::kOpenGL:
      return shaderc_target_env_opengl;
  }
}

shaderc_env_version GetEnvVersion(TargetEnv env) {
  switch (env) {
    case TargetEnv::kVulkan:
      return shaderc_env_version_vulkan_1_3;
    case TargetEnv::kOpenGL:
      return shaderc_env_version_opengl_4_5;
  }
}

shaderc_shader_kind GetShaderKind(ShaderKind kind) {
  switch (kind) {
    case ShaderKind::kVertex:
      return shaderc_vertex_shader;
    case ShaderKind::kFragment:
      return shaderc_fragment_shader;
  }
}

std::string LoadShaderSource(const std::filesystem::path& path) {
  if (!std::filesystem::exists(path)) {
    std::cerr << "Shader file not found at: " << path.string() << "\n";
    return "";
  }

  const std::string include_identifier = "#include ";
  static bool is_recursive_call = false;

  std::string full_source_code;
  std::ifstream file(path);

  if (!file.is_open()) {
    std::cerr << "Could not open the shader at: " << path.string() << "\n";
    return full_source_code;
  }

  std::string line_buffer;
  while (std::getline(file, line_buffer)) {
    if (line_buffer.find(include_identifier) != std::string::npos) {
      line_buffer.erase(0, include_identifier.size());

      line_buffer.erase(0, 1);
      line_buffer.erase(line_buffer.size() - 1);

      std::filesystem::path p = path.parent_path();
      line_buffer.insert(0, p.string() + "/");

      is_recursive_call = true;
      full_source_code += LoadShaderSource(line_buffer);

      continue;
    }

    full_source_code += line_buffer + '\n';
  }

  file.close();

  return full_source_code;
}

std::vector<uint32_t> CompileShader(const std::string& source,
                                    const char* file_name, ShaderKind kind,
                                    TargetEnv env, std::string definitions) {
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;

  auto target_env = GetTargetEnv(env);

  options.SetTargetEnvironment(target_env, GetEnvVersion(env));
  options.SetOptimizationLevel(shaderc_optimization_level_performance);

  if (!definitions.empty()) {
    options.AddMacroDefinition(definitions);
  }

  shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
      source, GetShaderKind(kind), file_name, options);
  if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
    std::cerr << module.GetErrorMessage() << "\n";
    return {};
  }

  return std::vector<uint32_t>(module.cbegin(), module.cend());
}
