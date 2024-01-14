// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/layer.h"

namespace eve {

struct ImGuiSettings {
  bool enable_viewports = true;
  bool enable_docking = true;
};

class ImGuiLayer : public Layer {
 public:
  ImGuiLayer(Ref<State>& state);
  ~ImGuiLayer() = default;

  void OnStart() override;

  void OnDestroy() override;

  void Begin();

  void End();

  ImGuiSettings& GetSettings() { return settings_; }

 private:
  static void SetDarkThemeColors();

 private:
  ImGuiSettings settings_{};
};

}  // namespace eve