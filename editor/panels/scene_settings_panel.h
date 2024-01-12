// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "graphics/scene_renderer.h"
#include "scene/editor_camera.h"
#include "scene/scene.h"

#include "ui/panel.h"

namespace eve {

class SceneSettingsPanel : public Panel {
  EVE_IMPL_PANEL("Scene Settings")

 public:
  SceneSettingsPanel(Ref<SceneRenderer> scene_renderer,
                     Ref<EditorCamera> camera);

 protected:
  void Draw() override;

 private:
  Ref<Scene> scene_ = nullptr;
  Ref<SceneRenderer> scene_renderer_;
  Ref<EditorCamera> camera_;
};

}  // namespace eve
