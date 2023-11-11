// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/panel.h"
#include "panel.h"

Panel::Panel(const PanelProperties& props)
    : is_active_(props.default_active), panel_size_(props.size) {}

void Panel::Render() {
  if (is_active_) {
    Begin();
    Draw();
    End();
  }
}

void Panel::PushStyle(ImGuiStyleVar style_var,
                      std::variant<ImVec2, float> style) {
  styles_[style_var] = style;
}

void Panel::SetFlags(ImGuiWindowFlags flags) {
  window_flags_ = flags;
}

void Panel::Begin() {
  if (styles_.size() > 0) {
    for (auto [style_var, value] : styles_) {
      if (std::holds_alternative<ImVec2>(value)) {
        ImGui::PushStyleVar(style_var, std::get<ImVec2>(value));
      } else if (std::holds_alternative<float>(value)) {
        ImGui::PushStyleVar(style_var, std::get<float>(value));
      }
    }
  }

  ImGui::Begin(GetName().c_str(), &is_active_, window_flags_);
  is_focused_ = ImGui::IsWindowFocused();
  is_hovered_ = ImGui::IsWindowHovered();

  ImVec2 size = ImGui::GetContentRegionAvail();
  panel_size_ = {size.x, size.y};
}

void Panel::End() {
  ImGui::End();
  if (styles_.size() > 0) {
    ImGui::PopStyleVar(styles_.size());
  }
}
