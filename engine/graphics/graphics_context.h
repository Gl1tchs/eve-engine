// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {
struct DeviceInformation {
  const char* vendor;
  const char* renderer;
};

class GraphicsContext {
 public:
  virtual void Init() = 0;

  virtual DeviceInformation GetDeviceInfo() const = 0;

  [[nodiscard]] static Ref<GraphicsContext> Create();
};
}  // namespace eve