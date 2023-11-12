// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <functional>

#define BIND_FUNC(fn)                                       \
  [this](auto&&... args) -> decltype(auto) {                \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }