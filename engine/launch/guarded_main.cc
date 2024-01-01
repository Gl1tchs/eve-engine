// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include <filesystem>

#include "core/core_minimal.h"
#include "core/debug/log.h"
#include "core/instance.h"

namespace eve {
extern Instance* CreateInstance(CommandLineArguments args);

int GuardedMain(CommandLineArguments args) {
  Logger::Init("engine.log");

  Instance* instance = CreateInstance(args);
  if (!instance) {
    return 1;
  }

  instance->StartEventLoop();

  delete instance;

  Logger::Deinit();

  return 0;
}
}  // namespace eve
