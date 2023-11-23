// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "modals/exit_modal.h"

#include <imgui.h>

#include "core/event/input.h"

ExitModal::ExitModal() : Modal("Exit", false) {
  SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
           ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings);
}

void ExitModal::Draw() {
  ImGui::Text("There are unsaved changes.");
  ImGui::Separator();
  if (ImGui::Button("Save and Exit")) {
    if (on_answer) {
      on_answer(ExitModalAnswer::kSaveAndExit);
    }
    ImGui::CloseCurrentPopup();
  }
  ImGui::SetItemDefaultFocus();
  ImGui::SameLine();

  if (ImGui::Button("Exit Without Saving")) {
    if (on_answer) {
      on_answer(ExitModalAnswer::kExitWithoutSaving);
    }
    ImGui::CloseCurrentPopup();
  }

  if (ImGui::Button("Cancel", ImVec2(-1, 0))) {
    if (on_answer) {
      on_answer(ExitModalAnswer::kCancel);
    }
    ImGui::CloseCurrentPopup();
  }

  if (Input::IsKeyPressed(KeyCode::kEscape)) {
    if (on_answer) {
      on_answer(ExitModalAnswer::kCancel);
    }
    ImGui::CloseCurrentPopup();
  }
}
