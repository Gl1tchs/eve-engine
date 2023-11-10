// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/core_minimal.h"
#include "core/instance.h"

extern Instance* CreateInstance(CommandLineArguments args);

int GuardedMain(CommandLineArguments args) {
  LoggerManager::Init("intermediate/engine.log");

  PROFILE_BEGIN_SESSION("Startup", "intermediate/eve-profiler-startup.prf");
  Instance* instance = CreateInstance(args);
  PROFILE_END_SESSION();

  PROFILE_BEGIN_SESSION("Runtime", "intermediate/eve-profiler-runtime.prf");
  instance->StartEventLoop();
  PROFILE_END_SESSION();

  PROFILE_BEGIN_SESSION("Shutdown", "intermediate/eve-profiler-shutdown.prf");
  delete instance;
  PROFILE_END_SESSION();

  return 0;
}