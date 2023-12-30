// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/window.h"
#include "graphics/renderer.h"

namespace eve {

struct State {
  Ref<Window> window;
  Ref<Renderer> renderer;
  bool running{true};
};

}  // namespace eve