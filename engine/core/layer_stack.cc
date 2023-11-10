// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/layer_stack.h"
#include "core/debug/instrumentor.h"

LayerStack::~LayerStack() {
  PROFILE_FUNCTION();

  for (Layer* layer : layers_) {
    layer->OnDestroy();
    delete layer;
  }
}

void LayerStack::UpdateLayers(float ds) {
  PROFILE_FUNCTION();

  for (Layer* layer : layers_) {
    layer->OnUpdate(ds);
  }
}

void LayerStack::UpdateGUI(float ds) {
  PROFILE_FUNCTION();

  for (Layer* layer : layers_) {
    layer->OnGUI(ds);
  }
}

void LayerStack::PushLayer(Layer* layer) {
  PROFILE_FUNCTION();

  layers_.emplace(layers_.begin() + layer_insert_idx_, layer);
  layer_insert_idx_++;

  layer->OnStart();
}

void LayerStack::PushOverlay(Layer* overlay) {
  PROFILE_FUNCTION();

  layers_.emplace_back(overlay);

  overlay->OnStart();
}

void LayerStack::PopLayer(Layer* layer) {
  PROFILE_FUNCTION();

  auto it =
      std::find(layers_.begin(), layers_.begin() + layer_insert_idx_, layer);
  if (it != layers_.begin() + layer_insert_idx_) {
    layer->OnDestroy();
    layers_.erase(it);
    layer_insert_idx_--;
  }
}

void LayerStack::PopOverlay(Layer* overlay) {
  PROFILE_FUNCTION();

  auto it =
      std::find(layers_.begin() + layer_insert_idx_, layers_.end(), overlay);
  if (it != layers_.end()) {
    overlay->OnDestroy();
    layers_.erase(it);
  }
}
