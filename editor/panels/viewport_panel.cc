// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/viewport_panel.h"

#include <imgui.h>

ViewportPanel::ViewportPanel(Ref<FrameBuffer>& frame_buffer)
    : Panel({true, frame_buffer->GetSize()}), frame_buffer_(frame_buffer) {
  PushStyle(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
}

void ViewportPanel::Draw() {
  uint64_t textureID = frame_buffer_->GetTexture()->GetTextureID();
  ImGui::Image(reinterpret_cast<void*>(textureID),
               ImVec2{GetSize().x, GetSize().y}, ImVec2{0, 1}, ImVec2{1, 0});
}
