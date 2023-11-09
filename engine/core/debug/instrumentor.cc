// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/debug/instrumentor.h"

#include <sstream>

#include "core/debug/log.h"

void Instrumentor::BeginSession(const std::string& name,
                                const std::string& file_path) {
  std::lock_guard lock(mutex_);
  if (current_session_) {
    // If there is already a current session, then close it before beginning new one.
    // Subsequent profiling output meant for the original session will end up in the
    // newly opened session instead.  That's better than having badly formatted
    // profiling output.

    // Edge case: BeginSession() might be before Log::Init()
    if (LoggerManager::GetEngineLogger()) {
      LOG_ENGINE_ERROR(
          "Instrumentor::BeginSession('{0}') when session '{1}' already open.",
          name, current_session_->name);
    }

    InternalEndSession();
  }

  output_stream_.open(file_path);

  if (output_stream_.is_open()) {
    current_session_ = new InstrumentationSession({name});
    WriteHeader();
  } else {
    // Edge case: BeginSession() might be before Log::Init()
    if (LoggerManager::GetEngineLogger()) {
      LOG_ENGINE_ERROR("Instrumentor could not open results file '{0}'.",
                       file_path);
    }
  }
}

void Instrumentor::EndSession() {
  std::lock_guard lock(mutex_);
  InternalEndSession();
}

void Instrumentor::WriteProfile(const ProfileResult& result) {
  std::stringstream json;

  json << std::setprecision(3) << std::fixed;
  json << ",{";
  json << "\"cattegory\":\"function\",";
  json << "\"duration\":" << (result.elapsed_time.count()) << ',';
  json << "\"name\":\"" << result.name << "\",";
  // json << "\"phase\":\"X\",";
  // json << "\"process_id\":0,";
  json << "\"thread_id\":" << result.thread_id << ",";
  json << "\"time_stamp\":" << result.start.count();
  json << "}";

  std::lock_guard lock(mutex_);
  if (current_session_) {
    output_stream_ << json.str();
    output_stream_.flush();
  }
}

Instrumentor& Instrumentor::Instance() {
  static Instrumentor instance;
  return instance;
}

Instrumentor::Instrumentor() : current_session_(nullptr) {}

Instrumentor::~Instrumentor() {
  EndSession();
}

void Instrumentor::WriteHeader() {
  output_stream_ << "{\"other_data\": {},\"trace_events\":[{}";
  output_stream_.flush();
}

void Instrumentor::WriteFooter() {
  output_stream_ << "]}";
  output_stream_.flush();
}

void Instrumentor::InternalEndSession() {
  if (current_session_) {
    WriteFooter();
    output_stream_.close();
    delete current_session_;
    current_session_ = nullptr;
  }
}

InstrumentationTimer::InstrumentationTimer(const std::string& name)
    : name_(name), stopped_(false) {
  start_time_point_ = std::chrono::steady_clock::now();
}

InstrumentationTimer::~InstrumentationTimer() {
  if (!stopped_) {
    Stop();
  }
}

void InstrumentationTimer::Stop() {
  auto end_time_point = std::chrono::steady_clock::now();
  auto high_res_start =
      microseconds_double{start_time_point_.time_since_epoch()};
  auto elapsed_time =
      std::chrono::time_point_cast<microseconds>(end_time_point)
          .time_since_epoch() -
      std::chrono::time_point_cast<microseconds>(start_time_point_)
          .time_since_epoch();

  Instrumentor::Instance().WriteProfile(
      {name_, high_res_start, elapsed_time, std::this_thread::get_id()});

  stopped_ = true;
}
