// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/layer_stack.h"

#include "core/instance.h"

namespace eve {

LayerStack::~LayerStack() {
  for (Layer* layer : layers_) {
    layer->OnDestroy();
    delete layer;
  }
}

void LayerStack::PushLayer(Layer* layer) {
  layers_.emplace(layers_.begin() + layer_insert_idx_, layer);
  layer_insert_idx_++;

  Instance::Get().EnqueueMain([layer]() { layer->OnStart(); });
}

void LayerStack::PushOverlay(Layer* overlay) {
  layers_.emplace_back(overlay);

  Instance::Get().EnqueueMain([overlay]() { overlay->OnStart(); });
}

}  // namespace eve