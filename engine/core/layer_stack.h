// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <cstdint>
#include <vector>

#include "core/layer.h"

class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void UpdateLayers(float ds);

  void UpdateGUI(float ds);

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

 private:
  std::vector<Layer*> layers_;
  uint32_t layer_insert_idx_ = 0;
};
