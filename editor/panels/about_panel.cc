// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/about_panel.h"

#include <imgui.h>

namespace eve {

AboutPanel::AboutPanel() {
  SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
}

void AboutPanel::Draw() {
  ImGui::TextUnformatted("Authors:");
  ImGui::Indent();
  ImGui::TextUnformatted("- Berke Umut Biricik");
  ImGui::Unindent();

  ImGui::TextUnformatted("Version: 0.1.0");

  ImGui::TextUnformatted("Icons By: icons8.com");
}

}  // namespace eve