// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/state.h"
#include "core/utils/memory.h"

class Layer {
 public:
  Layer(Ref<State>& state);
  virtual ~Layer() = default;

 private:
  virtual void OnStart() {}

  virtual void OnDestroy() {}

  virtual void OnUpdate(float ds) {}

  virtual void OnGUI(float ds) {}

 protected:
  Ref<State> state_;

  friend class Instance;
  friend class LayerStack;
};
