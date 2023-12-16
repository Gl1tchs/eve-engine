// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/toolbar_panel.h"

#include <IconsFontAwesome4.h>
#include <imgui.h>

#include "utils/imgui_utils.h"

ToolbarPanel::ToolbarPanel() {
  SetFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
           ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
}

void ToolbarPanel::Draw() {
  switch (state_) {
    case SceneState::kEdit: {
      if (ImGui::ButtonTransparent(ICON_FA_PLAY) && on_play) {
        on_play();
      }
      break;
    }
    case SceneState::kPlay: {
      if (ImGui::ButtonTransparent(ICON_FA_PAUSE) && on_pause) {
        on_pause();
      }
      ImGui::SameLine();
      if (ImGui::ButtonTransparent(ICON_FA_STOP) && on_stop) {
        on_stop();
      }
      break;
    }
    case SceneState::kPaused: {
      if (ImGui::ButtonTransparent(ICON_FA_PLAY) && on_resume) {
        on_resume();
      }
      ImGui::SameLine();
      if (ImGui::ButtonTransparent(ICON_FA_STEP_FORWARD) && on_step) {
        on_step();
      }
      ImGui::SameLine();
      if (ImGui::ButtonTransparent(ICON_FA_EJECT) && on_eject) {
        on_eject();
      }
      ImGui::SameLine();
      if (ImGui::ButtonTransparent(ICON_FA_STOP) && on_stop) {
        on_stop();
      }
      break;
    }
  }
}
