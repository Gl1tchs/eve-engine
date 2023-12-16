// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include <filesystem>

#include "core/core_minimal.h"
#include "core/debug/log.h"
#include "core/instance.h"

extern Instance* CreateInstance(CommandLineArguments args);

int GuardedMain(CommandLineArguments args) {
  if (!std::filesystem::exists(".eve/")) {
    std::filesystem::create_directory(".eve/");
  }

  Logger::Init(".eve/engine.log");

  Instance* instance = CreateInstance(args);
  if (!instance) {
    LOG_FATAL("Unable to create application!");
    return 1;
  }

  instance->StartEventLoop();

  delete instance;

  Logger::Deinit();

  return 0;
}