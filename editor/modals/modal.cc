// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "modals/modal.h"

#include <imgui.h>

#include "core/debug/log.h"

Modal::Modal(const std::string name, bool closable)
    : name_(name), closable_(closable) {
}

void Modal::Show() {
  ImGui::OpenPopup(name_.c_str());

  bool* p_open = nullptr;
  if (closable_) {
    p_open = new bool(true);
  }

  if (ImGui::BeginPopupModal(name_.c_str(), nullptr, window_flags_)) {
    Draw();
    ImGui::EndPopup();
  }

  if (p_open) {
    delete p_open;
  }
}
