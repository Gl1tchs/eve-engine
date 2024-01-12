// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "ui/modal.h"

#include <imgui.h>
#include "modal.h"

namespace eve {

Modal::Modal(bool external_state) : external_state_(external_state) {}

void Modal::Render() {
  if (!external_state_ && !should_show_) {
    return;
  }

  if (ImGui::BeginPopupModal(GetName().c_str(), nullptr, window_flags_)) {
    Draw();
    ImGui::EndPopup();
  }

  ImGui::OpenPopup(GetName().c_str());
}

}  // namespace eve