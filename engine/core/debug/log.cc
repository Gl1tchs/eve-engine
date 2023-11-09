// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/debug/log.h"

#include <unordered_map>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

std::unordered_map<std::string, Ref<spdlog::logger>> loggers;
std::vector<spdlog::sink_ptr> log_sinks;

void LogInstance::Trace(const std::string& fmt) {
  loggers[target]->trace(fmt);
}

void LogInstance::Info(const std::string& fmt) {
  loggers[target]->info(fmt);
}

void LogInstance::Warning(const std::string& fmt) {
  loggers[target]->warn(fmt);
}

void LogInstance::Error(const std::string& fmt) {
  loggers[target]->error(fmt);
}

void LogInstance::Critical(const std::string& fmt) {
  loggers[target]->critical(fmt);
}

Ref<LogInstance> LoggerManager::engine_logger_ = nullptr;

void LoggerManager::Init(const std::string& file_path) {
  log_sinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());

  log_sinks.emplace_back(
      CreateRef<spdlog::sinks::basic_file_sink_mt>(file_path, true));

  log_sinks[0]->set_pattern("[%T] %^[%n] [%l]%$: %v");
  log_sinks[1]->set_pattern("[%T] [%n] [%l]: %v");

  engine_logger_ = AddLogger("ENGINE");
}

Ref<LogInstance> LoggerManager::AddLogger(const std::string& target) {
  Ref<LogInstance> log = CreateRef<LogInstance>(target);

  Ref<spdlog::logger> spdlogger =
      CreateRef<spdlog::logger>(target, log_sinks.begin(), log_sinks.end());
  spdlog::register_logger(spdlogger);
  spdlogger->set_level(spdlog::level::trace);
  spdlogger->flush_on(spdlog::level::trace);

  loggers[log->target] = spdlogger;

  return log;
}

Ref<LogInstance> LoggerManager::GetEngineLogger() {
  return engine_logger_;
}