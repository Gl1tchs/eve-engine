// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "scene/editor_camera.h"
#include "scene/scene.h"


#include "panels/panel.h"

namespace eve {

class SceneSettingsPanel : public Panel {
  IMPL_PANEL("Scene Settings")

 public:
  SceneSettingsPanel(Ref<EditorCamera> camera);

 protected:
  void Draw() override;

 private:
  Ref<Scene> scene_ = nullptr;
  Ref<EditorCamera> camera_;
};

}  // namespace eve
