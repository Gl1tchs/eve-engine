// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/debug/log.h"
#include "core/layer.h"

class EditorLayer : public Layer {
 public:
  EditorLayer();
  ~EditorLayer();

 private:
  void OnStart() override;

  void OnDestroy() override;

  void OnUpdate(float ds) override;

  void OnGUI(float ds) override;

 private:
  Ref<LogInstance> editor_logger_;
};
