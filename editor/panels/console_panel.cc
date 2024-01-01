// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/console_panel.h"

#include <imgui.h>

namespace eve {

void PushButtonActivityColor() {
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
}

ConsolePanel::ConsolePanel() : Panel(true) {
  buffer_ = CreateRef<LogBuffer>(1000);
  Logger::PushBuffer(buffer_);
}

void ConsolePanel::Clear() {
  buffer_->Clear();
}

bool ConsolePanel::LevelInFilter(LogLevel level) {
  switch (level) {
    case LogLevel::kTrace:
      return LevelInFilter(LogLevelFilter_Trace);
    case LogLevel::kInfo:
      return LevelInFilter(LogLevelFilter_Info);
    case LogLevel::kWarning:
      return LevelInFilter(LogLevelFilter_Warning);
    case LogLevel::kError:
      return LevelInFilter(LogLevelFilter_Error);
    case LogLevel::kFatal:
      return LevelInFilter(LogLevelFilter_Fatal);
    default:
      return false;
  }

  return false;
}

bool ConsolePanel::LevelInFilter(LogLevelFilter filter) {
  return (level_filters_ & filter) != 0;
}

bool ConsolePanel::SenderInFilter(LogSender sender) {
  switch (sender) {
    case LogSender::kEngine:
      return SenderInFilter(LogSenderFilter_Engine);
    case LogSender::kEditor:
      return SenderInFilter(LogSenderFilter_Editor);
    case LogSender::kClient:
      return SenderInFilter(LogSenderFilter_Client);
    default:
      return false;
  }

  return false;
}

bool ConsolePanel::SenderInFilter(LogSenderFilter filter) {
  return (sender_filters_ & filter) != 0;
}

void ConsolePanel::DrawSenderFilterSelectable(const std::string& name,
                                              LogSenderFilter filter) {
  bool use_color = SenderInFilter(filter);
  if (use_color) {
    PushButtonActivityColor();
  }

  if (ImGui::SmallButton(name.c_str())) {
    sender_filters_ ^= filter;
  }

  if (use_color) {
    ImGui::PopStyleColor();
  }
}

void ConsolePanel::DrawLevelFilterSelectable(const std::string& name,
                                             LogLevelFilter filter) {
  bool use_color = LevelInFilter(filter);
  if (use_color) {
    PushButtonActivityColor();
  }

  if (ImGui::SmallButton(name.c_str())) {
    level_filters_ ^= filter;
  }

  if (use_color) {
    ImGui::PopStyleColor();
  }
}

void ConsolePanel::Draw() {
  if (!buffer_) {
    return;
  }

  const float footer_height_to_reserve =
      ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

  if (ImGui::SmallButton("Clear")) {
    Clear();
  }

  ImGui::SameLine();

  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &auto_scroll_);
    ImGui::EndPopup();
  }

  // Options, Filter
  if (ImGui::SmallButton("Options"))
    ImGui::OpenPopup("Options");

  ImGui::SameLine();
  ImGui::TextUnformatted("|");
  ImGui::SameLine();

  DrawSenderFilterSelectable("Engine", LogSenderFilter_Engine);
  ImGui::SameLine();
  DrawSenderFilterSelectable("Editor", LogSenderFilter_Editor);
  ImGui::SameLine();
  DrawSenderFilterSelectable("Client", LogSenderFilter_Client);

  ImGui::SameLine();
  ImGui::TextUnformatted("|");
  ImGui::SameLine();

  DrawLevelFilterSelectable("Trace", LogLevelFilter_Trace);
  ImGui::SameLine();
  DrawLevelFilterSelectable("Info", LogLevelFilter_Info);
  ImGui::SameLine();
  DrawLevelFilterSelectable("Warning", LogLevelFilter_Warning);
  ImGui::SameLine();
  DrawLevelFilterSelectable("Error", LogLevelFilter_Error);
  ImGui::SameLine();
  DrawLevelFilterSelectable("Fatal", LogLevelFilter_Fatal);

  ImGui::Separator();

  if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve),
                        ImGuiChildFlags_None,
                        ImGuiWindowFlags_HorizontalScrollbar)) {
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::Selectable("Clear"))
        Clear();
      ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(4, 1));  // Tighten spacing

    int i = 0;
    for (const LogMessage& message : *buffer_) {
      if (!LevelInFilter(message.level) || !SenderInFilter(message.sender)) {
        continue;
      }

      ImVec4 color;
      bool has_color = false;
      switch (message.level) {
        case LogLevel::kTrace:
          has_color = false;
          break;
        case LogLevel::kInfo:
          has_color = true;
          color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
          break;
        case LogLevel::kWarning:
          has_color = true;
          color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
          break;
        case LogLevel::kError:
          has_color = true;
          color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
          break;
        case LogLevel::kFatal:
          has_color = true;
          color = ImVec4(0.86f, 0.08f, 0.24f, 1.0f);
          break;
        default:
          break;
      }

      if (has_color) {
        ImGui::PushStyleColor(ImGuiCol_Text, color);
      }

      std::string log =
          std::format("[{}] [{}] [{}] {}", message.time_stamp,
                      GetLogSenderString(message.sender),
                      GetLogLevelString(message.level), message.string);
      ImGui::PushID(i);
      if (ImGui::Selectable(log.c_str(), selected_log_idx_ == i)) {
        selected_log_idx_ = (selected_log_idx_ == i) ? -1 : i;
      }
      ImGui::PopID();

      if (has_color) {
        ImGui::PopStyleColor();
      }

      i++;
    }

    if (scroll_to_bottom_ ||
        (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
      ImGui::SetScrollHereY(1.0f);
    }

    scroll_to_bottom_ = false;

    ImGui::PopStyleVar();
  }

  ImGui::EndChild();
}

}  // namespace eve