// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/core_minimal.h"
#include "core/window.h"
#include "graphics/renderer.h"

class Instance {
 public:
  Instance();
  ~Instance();

  void StartEventLoop();

 private:
  bool is_running_{true};

  Ref<Window> window_;
  Ref<Renderer> renderer_;
};
