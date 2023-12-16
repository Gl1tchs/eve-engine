// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "graphics/renderer.h"

#include "panels/panel.h"
#include "utils/cpuid.h"

class DebugInfoPanel : public Panel {
  IMPL_PANEL("Debug Info")

 public:
  DebugInfoPanel(Ref<Renderer>& renderer);

 protected:
  void Draw() override;

 private:
  Ref<Renderer> renderer_;

  uint64_t system_memory_ = 0;
  CPUInfo cpu_info_;
};
