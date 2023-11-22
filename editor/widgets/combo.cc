// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "widgets/combo.h"

#include <format>

#include <imgui.h>

void Combo::Draw(const char* id, std::vector<std::string> elements,
                 std::function<void(const std::string&)> callback) {
  static std::string current_element = elements[0];

  if (ImGui::BeginCombo(id, current_element.c_str())) {
    for (std::string element : elements) {
      bool is_selected = (current_element == element);

      if (ImGui::Selectable(element.c_str(), is_selected)) {
        callback(element);
      }

      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}
