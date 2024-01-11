// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "panels/panel.h"

namespace eve {

enum class SceneState { kEdit, kPlay, kPaused };

class ToolbarPanel : public Panel {
  IMPL_PANEL("Toolbar")

 public:
  ToolbarPanel();

  std::function<void()> on_play;
  std::function<void()> on_stop;
  std::function<void()> on_pause;
  std::function<void()> on_resume;
  std::function<void()> on_step;
  std::function<void()> on_eject;

  void SetState(SceneState state) { state_ = state; }

 protected:
  void Draw() override;

 private:
  SceneState state_ = SceneState::kEdit;
};

}  // namespace eve
