// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {
enum class LogLevel {
  Trace = 0,
  Info,
  Warning,
  Error,
  Fatal,
};

class Logger {
 public:
  static void Init(const std::string& file_name);

  static void Deinit();

  static void Log(LogLevel level, const std::string& fmt);

 private:
  static std::string GetColoredMessage(const std::string& message,
                                       LogLevel level);

 private:
  static std::unordered_map<LogLevel, std::string> s_verbosity_colors;

  static std::ofstream s_log_file;

  static const char* s_log_level_strings[];
};
}  // namespace eve

#define LOG_TRACE(...) \
  ::eve::Logger::Log(::eve::LogLevel::Trace, std::format(__VA_ARGS__))
#define LOG_INFO(...) \
  ::eve::Logger::Log(::eve::LogLevel::Info, std::format(__VA_ARGS__))
#define LOG_WARNING(...) \
  ::eve::Logger::Log(::eve::LogLevel::Warning, std::format(__VA_ARGS__))
#define LOG_ERROR(...) \
  ::eve::Logger::Log(::eve::LogLevel::Error, std::format(__VA_ARGS__))
#define LOG_FATAL(...) \
  ::eve::Logger::Log(::eve::LogLevel::Fatal, std::format(__VA_ARGS__))

#define LOG_IF(level, condition, ...)       \
  if (condition) {                          \
    ::eve::Logger::Log(level, __VA_ARGS__); \
  }

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
    LOG_FATAL(msg, __VA_ARGS__);              \
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