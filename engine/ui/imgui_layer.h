// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/layer.h"

class ImGuiLayer : public Layer {
 public:
  ImGuiLayer(Ref<State>& state);
  ~ImGuiLayer() = default;

  void Begin();

  void End();

 private:
  void OnStart() override;

  void OnDestroy() override;

  void SetDarkThemeColors();
};
