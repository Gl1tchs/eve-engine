// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "graphics/renderer.h"

#include "ui/panel.h"
#include "utils/cpuid.h"

namespace eve {
class DebugInfoPanel : public Panel {
  EVE_IMPL_PANEL("Debug Info")

 public:
  DebugInfoPanel(Ref<Renderer>& renderer);

 protected:
  void Draw() override;

 private:
  Ref<Renderer> renderer_;

  uint64_t system_memory_ = 0;
  CPUInfo cpu_info_;
};
}  // namespace eve
