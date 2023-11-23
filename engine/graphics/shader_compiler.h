// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "graphics/renderer_api.h"

enum class ShaderKind { kVertex, kFragment, kGeometry };

std::string LoadShaderSource(const std::filesystem::path& path);

std::vector<uint32_t> CompileShader(const std::string& source,
                                    const char* file_name, ShaderKind kind,
                                    GraphicsAPI api,
                                    std::string definitions = "");

std::optional<GraphicsAPI> DeserializeTargetEnv(std::string env);

std::optional<ShaderKind> DeserializeShaderKind(std::string kind);
