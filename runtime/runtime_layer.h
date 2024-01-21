// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "core/layer.h"
#include "graphics/scene_renderer.h"

namespace eve {

class RuntimeLayer : public Layer {
 public:
  RuntimeLayer(Ref<State>& state);

  virtual ~RuntimeLayer() = default;

  void OnCreate() override;

  void OnDestroy() override;

  void OnUpdate(float ds) override;

 private:
  Scope<SceneRenderer> scene_renderer_;
};

}  // namespace eve
