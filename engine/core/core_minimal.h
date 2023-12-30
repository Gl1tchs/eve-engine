// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/debug/log.h"
#include "core/utils/memory.h"

namespace eve {

struct CommandLineArguments {
  int argc;
  char** argv;
};

}  // namespace eve

#define BIND_FUNC(fn)                                       \
  [this](auto&&... args) -> decltype(auto) {                \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }
