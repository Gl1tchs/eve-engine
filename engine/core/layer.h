// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/state.h"

namespace eve {

class Layer {
 public:
  Layer(Ref<State>& state);
  virtual ~Layer() = default;

  virtual void OnStart() {}

  virtual void OnDestroy() {}

  virtual void OnUpdate(float ds) {}

  virtual void OnGUI(float ds) {}

 protected:
  Ref<State>& GetState() { return state_; }
  const Ref<State>& GetState() const { return state_; }

 private:
  Ref<State> state_;

  friend class Instance;
  friend class LayerStack;
};

}  // namespace eve