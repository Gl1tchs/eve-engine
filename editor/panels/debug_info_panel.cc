// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/debug_info_panel.h"

#if _WIN32
#include <Windows.h>
#endif

#include <imgui.h>

#include "graphics/graphics_context.h"

namespace eve {
DebugInfoPanel::DebugInfoPanel(Ref<Renderer>& renderer)
    : Panel(false), renderer_(renderer) {
  SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

#if _WIN32
  MEMORYSTATUSEX status;
  status.dwLength = sizeof(status);
  GlobalMemoryStatusEx(&status);
  system_memory_ = status.ullTotalPhys;
#elif __linux__
  // TODO testing required
  long pages = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);
  system_memory_ = pages * page_size;
#endif
}

void DebugInfoPanel::Draw() {
  const DeviceInformation info = renderer_->GetContext()->GetDeviceInfo();
  const RenderStats& stats = renderer_->GetStats();

  ImGui::SeparatorText("GPU:");
  ImGui::Text("Vendor: %s", info.vendor);
  ImGui::Text("Renderer: %s", info.renderer);

  ImGui::SeparatorText("CPU:");
  ImGui::Text("Vendor: %s", cpu_info_.GetVendor().c_str());
  ImGui::Text("Model: %s", cpu_info_.GetModel().c_str());
  ImGui::Text("Core Count: %d", cpu_info_.GetCores());
  ImGui::Text("Logical CPU Count: %d", cpu_info_.GetLogicalCpuCount());
  ImGui::Text("HyperThread: %d", cpu_info_.IsHyperThreaded());

  ImGui::SeparatorText("RAM:");
  ImGui::Text("Memory: %llu", system_memory_);

  ImGui::SeparatorText("Renderer Stats:");
  ImGui::Text("Last Render Duration: %.3f", stats.last_render_duration);
  ImGui::Text("Draw Calls: %d", stats.draw_calls);
  ImGui::Text("Vertex Count: %d", stats.vertex_count);
  ImGui::Text("Index Count: %d", stats.index_count);
}
}  // namespace eve
