// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/utils/memory.h"

class GraphicsContext {
 public:
  virtual void Init() = 0;

  [[nodiscard]] static Ref<GraphicsContext> Create();
};
