// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "asset/asset_library.h"
#include "core/window.h"
#include "graphics/renderer.h"

struct State {
  Ref<Window> window;
  Ref<Renderer> renderer;
  Ref<AssetLibrary> asset_library;
  bool running{true};
};
