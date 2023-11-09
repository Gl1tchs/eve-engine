// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_CORE_DEBUG_INSTRUMENTOR_H_
#define EVE_CORE_DEBUG_INSTRUMENTOR_H_

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

namespace {
typedef std::chrono::duration<double, std::micro> microseconds_double;
typedef std::chrono::duration<long long, std::micro> microseconds;
}  // namespace

struct ProfileResult final {
  std::string name;
  microseconds_double start;
  microseconds elapsed_time;
  std::thread::id thread_id;
};

struct InstrumentationSession final {
  std::string name;
};

class Instrumentor final {
 public:
  Instrumentor(const Instrumentor&) = delete;
  Instrumentor(Instrumentor&&) = delete;

  void BeginSession(const std::string& name,
                    const std::string& file_path = "profiler_result.json");

  void EndSession();

  void WriteProfile(const ProfileResult& result);

  static Instrumentor& Instance();

 private:
  Instrumentor();
  ~Instrumentor();

  void WriteHeader();

  void WriteFooter();

  void InternalEndSession();

 private:
  std::mutex mutex_;
  InstrumentationSession* current_session_;
  std::ofstream output_stream_;
};

class InstrumentationTimer final {
 public:
  InstrumentationTimer(const std::string& name);
  ~InstrumentationTimer();

  void Stop();

 private:
  const std::string name_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_point_;
  bool stopped_;
};

namespace instrumentor_utils {

template <size_t N>
struct ChangeResult {
  char data[N];
};

template <size_t N, size_t K>
constexpr auto CleanupOutputString(const char (&expr)[N],
                                   const char (&remove)[K]);

}  // namespace instrumentor_utils

#if EVE_ENABLE_PROFILING
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define FUNCTION_SIGNATURE __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define FUNCTION_SIGNATURE __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define FUNCTION_SIGNATURE __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define FUNCTION_SIGNATURE __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define FUNCTION_SIGNATURE __func__
#else
#define FUNCTION_SIGNATURE "FUNCTION_SIGNATURE unknown!"
#endif

#define PROFILE_BEGIN_SESSION(name, file_path) \
  Instrumentor::Instance().BeginSession(name, file_path)
#define PROFILE_END_SESSION() Instrumentor::Instance().EndSession()
#define PROFILE_SCOPE_LINE2(name, line)                          \
  constexpr auto fixed_name_##line =                             \
      instrumentor_utils::CleanupOutputString(name, "__cdecl "); \
  InstrumentationTimer timer_##line(fixed_name_##line.data)
#define PROFILE_SCOPE_LINE(name, line) PROFILE_SCOPE_LINE2(name, line)
#define PROFILE_SCOPE(name) PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(FUNCTION_SIGNATURE)
#else
#define PROFILE_BEGIN_SESSION(name, file_path)
#define PROFILE_END_SESSION()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

#include "core/debug/instrumentor.inc"

#endif  // EVE_CORE_DEBUG_INSTRUMENTOR_H_
