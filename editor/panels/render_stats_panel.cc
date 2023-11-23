// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/render_stats_panel.h"

#include <imgui.h>

RenderStatsPanel::RenderStatsPanel(Ref<Renderer>& renderer)
    : Panel({false, {0, 0}}), renderer_(renderer) {
  SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
}

void RenderStatsPanel::Draw() {
  const RenderStats& stats = renderer_->GetStats();

  ImGui::Text("Renderer Stats:");
  ImGui::Text("Draw Calls: %d", stats.draw_calls);
  ImGui::Text("Vertices: %d", stats.vertex_count);
  ImGui::Text("Indices: %d", stats.index_count);
}
