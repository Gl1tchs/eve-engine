// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/debug/log.h"
#include "core/utils/memory.h"

struct CommandLineArguments {
  int argc;
  char** argv;
};

#define BIND_FUNC(fn)                                       \
  [this](auto&&... args) -> decltype(auto) {                \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }
