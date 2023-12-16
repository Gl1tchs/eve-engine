// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "graphics/renderer.h"

#include "panels/panel.h"

class RenderStatsPanel : public Panel {
  IMPL_PANEL("Render Stats")

 public:
  RenderStatsPanel(Ref<Renderer>& renderer);

 protected:
  void Draw() override;

 private:
  Ref<Renderer> renderer_;
};
