// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/core_minimal.h"
#include "graphics/renderer_api.h"

namespace eve {
constexpr GraphicsAPI GetGraphicsAPI() {
  return GraphicsAPI::kOpenGL;
}
}  // namespace eve