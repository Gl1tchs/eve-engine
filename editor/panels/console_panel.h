// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "panels/panel.h"

namespace eve {

enum LogSenderFilter : uint16_t {
  LogSenderFilter_Engine = 1 << 0,
  LogSenderFilter_Editor = 1 << 1,
  LogSenderFilter_Client = 1 << 2
};

enum LogLevelFilter : uint16_t {
  LogLevelFilter_Trace = 1 << 0,
  LogLevelFilter_Info = 1 << 1,
  LogLevelFilter_Warning = 1 << 2,
  LogLevelFilter_Error = 1 << 3,
  LogLevelFilter_Fatal = 1 << 4
};

class ConsolePanel : public Panel {
  IMPL_PANEL("Console")

 public:
  ConsolePanel();

  void Clear();

 private:
  bool LevelInFilter(LogLevel level);

  bool LevelInFilter(LogLevelFilter filter);

  bool SenderInFilter(LogSender sender);

  bool SenderInFilter(LogSenderFilter filter);

  void DrawSenderFilterSelectable(const std::string& name,
                                  LogSenderFilter filter);

  void DrawLevelFilterSelectable(const std::string& name,
                                 LogLevelFilter filter);

 protected:
  void Draw() override;

 private:
  Ref<LogBuffer> buffer_;

  bool auto_scroll_ = true;
  bool scroll_to_bottom_ = false;
  int selected_log_idx_ = -1;

  uint16_t sender_filters_ =
      LogSenderFilter_Engine | LogSenderFilter_Editor | LogSenderFilter_Client;

  uint16_t level_filters_ = LogLevelFilter_Trace | LogLevelFilter_Info |
                            LogLevelFilter_Warning | LogLevelFilter_Error |
                            LogLevelFilter_Fatal;
};
}  // namespace eve
