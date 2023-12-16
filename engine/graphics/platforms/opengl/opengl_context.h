// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/graphics_context.h"

class OpenGLContext final : public GraphicsContext {
 public:
  void Init() override;

  DeviceInformation GetDeviceInfo() const override;
};
