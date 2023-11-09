// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_CORE_DEBUG_ASSERT_H_
#define EVE_CORE_DEBUG_ASSERT_H_

#include <filesystem>

#include "core/debug/log.h"

#if defined(EVE_PLATFORM_WIN32)
#define DEBUGBREAK() __debugbreak()
#elif defined(EVE_PLATFORM_LINUX)
#include <signal.h>
#define DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

#ifdef EVE_DEBUG
#define INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
  {                                                 \
    if (!(check)) {                                 \
      LOG##type##ERROR(msg, __VA_ARGS__);           \
      DEBUGBREAK();                                 \
    }                                               \
  }
#define INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
  INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define INTERNAL_ASSERT_NO_MSG(type, check)                                 \
  INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}",    \
                       STRINGIFY_MACRO(check),                              \
                       std::filesystem::path(__FILE__).filename().string(), \
                       __LINE__)

#define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define INTERNAL_ASSERT_GET_MACRO(...)         \
  EXPAND_MACRO(INTERNAL_ASSERT_GET_MACRO_NAME( \
      __VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the
// message) being optional
#define ASSERT(...) \
  EXPAND_MACRO(INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define ENGINE_ASSERT(...) \
  EXPAND_MACRO(INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_ENGINE_, __VA_ARGS__))
#else
#define ASSERT(...)
#define ENGINE_ASSERT(...)
#endif

#endif  // EVE_CORE_DEBUG_ASSERT_H_