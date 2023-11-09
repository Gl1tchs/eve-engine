// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <filesystem>
#include <format>
#include <string>

#include "core/utils/memory.h"

struct LogInstance final {
  std::string target;

  void Trace(const std::string& fmt);
  void Info(const std::string& fmt);
  void Warning(const std::string& fmt);
  void Error(const std::string& fmt);
  void Critical(const std::string& fmt);
};

class LoggerManager final {
 public:
  static void Init(const std::string& file_path);

  static Ref<LogInstance> AddLogger(const std::string& target);

  static Ref<LogInstance> GetEngineLogger();

 private:
  static Ref<LogInstance> engine_logger_;
};

#define NEW_LOG_INSTANCE(name) \
  const Ref<LogInstance> name = LoggerManager::AddLogger(#name);

#define LOG_ENGINE_TRACE(...) \
  LoggerManager::GetEngineLogger()->Trace(std::format(__VA_ARGS__));
#define LOG_ENGINE_INFO(...) \
  LoggerManager::GetEngineLogger()->Info(std::format(__VA_ARGS__));
#define LOG_ENGINE_WARNING(...) \
  LoggerManager::GetEngineLogger()->Warning(std::format(__VA_ARGS__));
#define LOG_ENGINE_ERROR(...) \
  LoggerManager::GetEngineLogger()->Error(std::format(__VA_ARGS__));
#define LOG_ENGINE_CRITICAL(...) \
  LoggerManager::GetEngineLogger()->Critical(std::format(__VA_ARGS__));

#define LOG_TRACE(instance, ...) instance->Trace(std::format(__VA_ARGS__));
#define LOG_INFO(instance, ...) instance->Info(std::format(__VA_ARGS__));
#define LOG_WARNING(instance, ...) instance->Warning(std::format(__VA_ARGS__));
#define LOG_ERROR(instance, ...) instance->Error(std::format(__VA_ARGS__));
#define LOG_CRITICAL(instance, ...) \
  instance->Critical(std::format(__VA_ARGS__));
