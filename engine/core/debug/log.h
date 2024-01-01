// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <cstdint>
#include <string>

#include "core/utils/memory.h"

namespace eve {

enum class LogSender { kEngine, kEditor, kRuntime, kClient };

enum class LogLevel {
  kTrace = 0,
  kInfo,
  kWarning,
  kError,
  kFatal,
};

std::string GetLogLevelString(LogLevel level);

struct LogMessage {
  LogSender sender;
  LogLevel level;
  std::string time_stamp;
  std::string string;
};

std::string GetLogSenderString(LogSender sender);

class LogBuffer {
 public:
  LogBuffer(uint32_t max_messages);

  void Log(LogSender sender, LogLevel level, const std::string& time_stamp,
           const std::string& m1essage);

  void Clear();

  std::deque<LogMessage>::iterator begin() { return messages_.begin(); }
  std::deque<LogMessage>::iterator end() { return messages_.end(); }
  std::deque<LogMessage>::const_iterator begin() const {
    return messages_.begin();
  }
  std::deque<LogMessage>::const_iterator end() const { return messages_.end(); }

 private:
  uint32_t max_messages_ = 1000;
  std::deque<LogMessage> messages_;
};

class Logger {
 public:
  static void Init(const std::string& file_name);

  static void Deinit();

  static void Log(LogSender sender, LogLevel level, const std::string& fmt);

  static void PushBuffer(Ref<LogBuffer>& buffer);

 private:
  static std::string GetColoredMessage(const std::string& message,
                                       LogLevel level);

 private:
  static std::unordered_map<LogLevel, std::string> verbosity_colors_;

  static std::ofstream log_file_;
  static std::vector<Ref<LogBuffer>> log_buffers_;
};
}  // namespace eve

#define LOG_ENGINE_TRACE(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEngine, ::eve::LogLevel::kTrace, \
                     std::format(__VA_ARGS__))
#define LOG_ENGINE_INFO(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEngine, ::eve::LogLevel::kInfo, \
                     std::format(__VA_ARGS__))
#define LOG_ENGINE_WARNING(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEngine, ::eve::LogLevel::kWarning, \
                     std::format(__VA_ARGS__))
#define LOG_ENGINE_ERROR(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEngine, ::eve::LogLevel::kError, \
                     std::format(__VA_ARGS__))
#define LOG_ENGINE_FATAL(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEngine, ::eve::LogLevel::kFatal, \
                     std::format(__VA_ARGS__))

#define LOG_EDITOR_TRACE(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEditor, ::eve::LogLevel::kTrace, \
                     std::format(__VA_ARGS__))
#define LOG_EDITOR_INFO(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEditor, ::eve::LogLevel::kInfo, \
                     std::format(__VA_ARGS__))
#define LOG_EDITOR_WARNING(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEditor, ::eve::LogLevel::kWarning, \
                     std::format(__VA_ARGS__))
#define LOG_EDITOR_ERROR(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEditor, ::eve::LogLevel::kError, \
                     std::format(__VA_ARGS__))
#define LOG_EDITOR_FATAL(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kEditor, ::eve::LogLevel::kFatal, \
                     std::format(__VA_ARGS__))

#define LOG_RUNTIME_TRACE(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kRuntime, ::eve::LogLevel::kTrace, \
                     std::format(__VA_ARGS__))
#define LOG_RUNTIME_INFO(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kRuntime, ::eve::LogLevel::kInfo, \
                     std::format(__VA_ARGS__))
#define LOG_RUNTIME_WARNING(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kRuntime, ::eve::LogLevel::kWarning, \
                     std::format(__VA_ARGS__))
#define LOG_RUNTIME_ERROR(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kRuntime, ::eve::LogLevel::kError, \
                     std::format(__VA_ARGS__))
#define LOG_RUNTIME_FATAL(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kRuntime, ::eve::LogLevel::kFatal, \
                     std::format(__VA_ARGS__))

#define LOG_CLIENT_TRACE(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kClient, ::eve::LogLevel::kTrace, \
                     std::format(__VA_ARGS__))
#define LOG_CLIENT_INFO(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kClient, ::eve::LogLevel::kInfo, \
                     std::format(__VA_ARGS__))
#define LOG_CLIENT_WARNING(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kClient, ::eve::LogLevel::kWarning, \
                     std::format(__VA_ARGS__))
#define LOG_CLIENT_ERROR(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kClient, ::eve::LogLevel::kError, \
                     std::format(__VA_ARGS__))
#define LOG_CLIENT_FATAL(...)                                            \
  ::eve::Logger::Log(::eve::LogSender::kClient, ::eve::LogLevel::kFatal, \
                     std::format(__VA_ARGS__))

#if _WIN32
#define DEBUGBREAK() __debugbreak()
#elif __linux__
#include <signal.h>
#define DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

#define INTERNAL_ASSERT_IMPL(check, msg, ...) \
  if (!(check)) {                             \
    LOG_ENGINE_FATAL(msg, __VA_ARGS__);       \
    DEBUGBREAK();                             \
  }

#define INTERNAL_ASSERT_WITH_MSG(check, ...) \
  INTERNAL_ASSERT_IMPL(check, "Assertion failed: {}", __VA_ARGS__)

#define INTERNAL_ASSERT_NO_MSG(check)                           \
  INTERNAL_ASSERT_IMPL(check, "Assertion '{}' failed at {}:{}", \
                       STRINGIFY_MACRO(check),                  \
                       std::filesystem::path(__FILE__).string(), __LINE__)

#define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro

#define INTERNAL_ASSERT_GET_MACRO(...)         \
  EXPAND_MACRO(INTERNAL_ASSERT_GET_MACRO_NAME( \
      __VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG))

#define ASSERT(...) \
  EXPAND_MACRO(INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__))