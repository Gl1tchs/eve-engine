// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/core_minimal.h"
#include "runtime/instance.h"

int GuardedMain() {
  LoggerManager::Init("intermediate/engine.log");

  PROFILE_BEGIN_SESSION("Startup", "intermediate/eve-profiler-startup.prf");
  Instance* instance = new Instance();
  PROFILE_END_SESSION();

  PROFILE_BEGIN_SESSION("Runtime", "intermediate/eve-profiler-runtime.prf");
  instance->StartEventLoop();
  PROFILE_END_SESSION();

  PROFILE_BEGIN_SESSION("Shutdown", "intermediate/eve-profiler-shutdown.prf");
  delete instance;
  PROFILE_END_SESSION();

  return 0;
}