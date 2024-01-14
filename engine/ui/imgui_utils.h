// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/color.h"

namespace ImGui {

struct ScopedStyleColor {
  ScopedStyleColor() = default;

  ScopedStyleColor(int idx, const eve::Color& color, bool condition = true);

  ScopedStyleColor(int idx, uint32_t color, bool condition = true);

  ~ScopedStyleColor();

 private:
  bool set_ = false;
};

struct ScopedStyleVar {
  ScopedStyleVar() = default;

  ScopedStyleVar(int idx, const glm::vec2& var, bool condition = true);

  ~ScopedStyleVar();

 private:
  bool set_ = false;
};

bool ButtonTransparent(const std::string& text, float w = 0, float h = 0);

void DrawTreeNode(const std::string& text,
                  std::function<void(void)> ui_function);

}  // namespace ImGui
