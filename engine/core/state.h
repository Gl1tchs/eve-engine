// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/window.h"
#include "graphics/renderer.h"

struct State {
  Ref<Window> window;
  Ref<Renderer> renderer;
};
