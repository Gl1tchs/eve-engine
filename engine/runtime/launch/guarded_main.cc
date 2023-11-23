// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include <filesystem>

#include "core/core_minimal.h"
#include "core/instance.h"

extern Instance* CreateInstance(CommandLineArguments args);

int GuardedMain(CommandLineArguments args) {
  if (!std::filesystem::exists(".eve/")) {
    std::filesystem::create_directory(".eve/");
  }

  LoggerManager::Init(".eve/engine.log");

  PROFILE_BEGIN_SESSION("Startup", ".eve/eve-profiler-startup.prf");
  Instance* instance = CreateInstance(args);
  if (!instance) {
    LOG_ENGINE_CRITICAL("Unable to create application!");
    return 1;
  }
  PROFILE_END_SESSION();

  PROFILE_BEGIN_SESSION("Runtime", ".eve/eve-profiler-runtime.prf");
  instance->StartEventLoop();
  PROFILE_END_SESSION();

  PROFILE_BEGIN_SESSION("Shutdown", ".eve/eve-profiler-shutdown.prf");
  delete instance;
  PROFILE_END_SESSION();

  return 0;
}