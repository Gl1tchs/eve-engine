// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

enum class ShaderKind { kVertex, kFragment };
enum class TargetEnv { kVulkan, kOpenGL };

std::string LoadShaderSource(const std::filesystem::path& path);

std::vector<uint32_t> CompileShader(const std::string& source,
                                    const char* file_name, ShaderKind kind,
                                    TargetEnv env,
                                    std::string definitions = "");

std::optional<TargetEnv> DeserializeTargetEnv(std::string env);

std::optional<ShaderKind> DeserializeShaderKind(std::string kind);
