// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace ImGui {

struct ScopedStyleColor {
  ScopedStyleColor() = default;

  ScopedStyleColor(int idx, const glm::vec4& color, bool condition = true);

  ScopedStyleColor(int idx, uint32_t color, bool condition = true);

  ~ScopedStyleColor();

 private:
  bool set_ = false;
};

bool ButtonTransparent(const std::string& text, float w = 0, float h = 0);

}  // namespace ImGui
