// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "ui/imgui_utils.h"

#include <imgui.h>

namespace ImGui {

ScopedStyleColor::ScopedStyleColor(int idx, const glm::vec4& color,
                                   bool condition)
    : set_(condition) {
  if (condition) {
    ImGui::PushStyleColor(idx, ImVec4(color.r, color.g, color.b, color.a));
  }
}

ScopedStyleColor::ScopedStyleColor(int idx, uint32_t color, bool condition)
    : set_(condition) {
  if (condition) {
    ImGui::PushStyleColor(idx, color);
  }
}

ScopedStyleColor::~ScopedStyleColor() {
  if (set_) {
    ImGui::PopStyleColor();
  }
}

bool ButtonTransparent(const std::string& text, float w, float h) {
  // Save the current style
  ImGuiStyle& style = ImGui::GetStyle();
  ImGuiStyle backup_style = style;

  // Set the button text color to be the same as the background color
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_Button] = clear_color;
  // style.Colors[ImGuiCol_ButtonHovered] = clear_color;
  // style.Colors[ImGuiCol_ButtonActive] = clear_color;

  // Draw the transparent button
  bool pressed = ImGui::Button(text.c_str(), ImVec2(w, h));

  // Restore the original style
  ImGui::GetStyle() = backup_style;

  return pressed;
}

}  // namespace ImGui
