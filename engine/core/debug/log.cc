// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/debug/log.h"

namespace eve {

static std::string GetTimestamp() {
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm tm_now;
  localtime_s(&tm_now, &now);

  std::stringstream ss;
  ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

std::unordered_map<LogLevel, std::string> Logger::s_verbosity_colors = {
    {LogLevel::Trace, "\x1B[1m"},     // None
    {LogLevel::Info, "\x1B[32m"},     // Green
    {LogLevel::Warning, "\x1B[93m"},  // Yellow
    {LogLevel::Error, "\x1B[91m"},    // Light Red
    {LogLevel::Fatal, "\x1B[31m"},    // Red
};

std::ofstream Logger::s_log_file;

const char* Logger::s_log_level_strings[] = {"Trace", "Info", "Warning",
                                             "Error", "Fatal"};

void Logger::Init(const std::string& file_name) {
  s_log_file.open(file_name);
  if (!s_log_file.is_open()) {
    throw std::runtime_error(
        "Error: Unable to initialize logger file does not exists!\n");
  }
}

void Logger::Deinit() {
  if (!s_log_file.is_open()) {
    return;
  }

  s_log_file.close();
}

void Logger::Log(LogLevel level, const std::string& fmt) {
  std::string time_stamp = GetTimestamp();
  std::string level_str = s_log_level_strings[static_cast<size_t>(level)];

  std::string message =
      std::format("[{}] [{}]: \"{}\"", time_stamp, level_str, fmt);

  std::string colored_messages = GetColoredMessage(message, level);

  // Output to stdout
  std::cout << colored_messages << "\x1B[0m\n";

  // Output to file
  if (s_log_file.is_open()) {
    s_log_file << message << "\n";
  }
}

std::string Logger::GetColoredMessage(const std::string& message,
                                      LogLevel level) {
  auto color_it = s_verbosity_colors.find(level);
  if (color_it != s_verbosity_colors.end()) {
    return color_it->second + message;
  }

  return message;  // No color for the default case
}

}  // namespace eve
