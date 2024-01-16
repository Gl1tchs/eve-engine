// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/debug/log.h"
#include "log.h"

namespace eve {

std::string DeserializeLogLevel(LogLevel level) {
  switch (level) {
    case LogLevel::kTrace: {
      return "TRACE";
    }
    case LogLevel::kInfo: {
      return "INFO";
    }
    case LogLevel::kWarning: {
      return "WARNING";
    }
    case LogLevel::kError: {
      return "ERROR";
    }
    case LogLevel::kFatal: {
      return "FATAL";
    }
    default:
      return "";
  }
}

std::string DeserializeLogSender(LogSender sender) {
  switch (sender) {
    case LogSender::kEngine:
      return "ENGINE";
    case LogSender::kEditor:
      return "EDITOR";
    case LogSender::kRuntime:
      return "RUNTIME";
    case LogSender::kClient:
      return "CLIENT";
    default:
      return "";
  }
}

static std::string GetTimestamp() {
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm tm_now;
  localtime_s(&tm_now, &now);

  std::stringstream ss;
  ss << std::put_time(&tm_now, "%H:%M:%S");
  return ss.str();
}

std::unordered_map<LogLevel, std::string> Logger::verbosity_colors_ = {
    {LogLevel::kTrace, "\x1B[1m"},     // None
    {LogLevel::kInfo, "\x1B[32m"},     // Green
    {LogLevel::kWarning, "\x1B[93m"},  // Yellow
    {LogLevel::kError, "\x1B[91m"},    // Light Red
    {LogLevel::kFatal, "\x1B[31m"},    // Red
};

std::ofstream Logger::log_file_;

std::vector<Ref<LogBuffer>> Logger::log_buffers_ = {};

LogBuffer::LogBuffer(uint32_t max_messages) : max_messages_(max_messages) {}

void LogBuffer::Log(LogSender sender, LogLevel level,
                    const std::string& time_stamp, const std::string& message) {
  if (messages_.size() + 1 >= max_messages_) {
    messages_.pop_front();
  }

  messages_.push_back({sender, level, time_stamp, message});
}

void LogBuffer::Clear() {
  messages_.clear();
}

void Logger::Init(const std::string& file_name) {
  log_file_.open(file_name);
  if (!log_file_.is_open()) {
    throw std::runtime_error(
        "Error: Unable to initialize logger file does not exists!\n");
  }
}

void Logger::Deinit() {
  if (!log_file_.is_open()) {
    return;
  }

  log_file_.close();
}

void Logger::Log(LogSender sender, LogLevel level, const std::string& fmt) {
  std::string time_stamp = GetTimestamp();

  std::string message =
      std::format("[{}] [{}] [{}]: \"{}\"", time_stamp,
                  DeserializeLogSender(sender), DeserializeLogLevel(level), fmt);

  std::string colored_messages = GetColoredMessage(message, level);

  // Output to buffers
  for (auto& buffer : log_buffers_) {
    buffer->Log(sender, level, time_stamp, fmt);
  }

  // Output to stdout
  std::cout << colored_messages << "\x1B[0m\n";

  // Output to file
  if (log_file_.is_open()) {
    log_file_ << message << "\n";
  }
}

void Logger::PushBuffer(Ref<LogBuffer>& buffer) {
  log_buffers_.push_back(buffer);
}

std::string Logger::GetColoredMessage(const std::string& message,
                                      LogLevel level) {
  auto color_it = verbosity_colors_.find(level);
  if (color_it != verbosity_colors_.end()) {
    return color_it->second + message;
  }

  return message;  // No color for the default case
}

}  // namespace eve
