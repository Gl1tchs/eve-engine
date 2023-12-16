// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/panel.h"

#include <imgui.h>

ImVec2 VecToImVec(const glm::vec2& lhs) {
  return ImVec2(lhs.x, lhs.y);
}

glm::vec2 ImVecToVec(const ImVec2& lhs) {
  return glm::vec2(lhs.x, lhs.y);
}

Panel::Panel(bool default_active, glm::vec2 size, glm::vec2 pos)
    : is_active_(default_active), panel_size_(size), panel_pos_(pos) {}

void Panel::Render() {
  if (is_active_) {
    Begin();
    Draw();
    End();
  }
}

void Panel::PushStyle(int style_var, std::variant<glm::vec2, float> style) {
  styles_[style_var] = style;
}

void Panel::SetFlags(int flags) {
  window_flags_ = flags;
}

void Panel::Begin() {
  if (styles_.size() > 0) {
    for (auto [style_var, value] : styles_) {
      if (std::holds_alternative<glm::vec2>(value)) {
        const auto& value_glm = std::get<glm::vec2>(value);
        ImGui::PushStyleVar(style_var, ImVec2(value_glm.x, value_glm.y));
      } else if (std::holds_alternative<float>(value)) {
        ImGui::PushStyleVar(style_var, std::get<float>(value));
      }
    }
  }

  bool* p_active = nullptr;
  if (!is_static_) {
    p_active = &is_active_;
  } else {
    ImGui::SetNextWindowPos(VecToImVec(panel_pos_));
    ImGui::SetNextWindowPos(VecToImVec(panel_size_));
  }

  ImGui::Begin(GetName().c_str(), p_active, window_flags_);

  is_focused_ = ImGui::IsWindowFocused();
  is_hovered_ = ImGui::IsWindowHovered();

  if (!is_static_) {
    panel_size_ = ImVecToVec(ImGui::GetContentRegionAvail());
    panel_pos_ = ImVecToVec(ImGui::GetWindowPos());
  }
}

void Panel::End() {
  ImGui::End();
  if (styles_.size() > 0) {
    ImGui::PopStyleVar(styles_.size());
  }
}
