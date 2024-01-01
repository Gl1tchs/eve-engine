// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "core/layer.h"
#include "graphics/frame_buffer.h"
#include "graphics/scene_renderer.h"
#include "scene/editor_camera.h"
#include "scene/scene.h"

#include "modals/exit_modal.h"
#include "panels/console_panel.h"
#include "panels/debug_info_panel.h"
#include "panels/hierarchy_panel.h"
#include "panels/inspector_panel.h"
#include "panels/toolbar_panel.h"
#include "panels/viewport_panel.h"
#include "widgets/menu_bar.h"

namespace eve {
class EditorLayer : public Layer {
 public:
  EditorLayer(Ref<State>& state);
  virtual ~EditorLayer() = default;

 private:
  void OnStart() override;

  void OnDestroy() override;

  void OnUpdate(float ds) override;

  void OnGUI(float ds) override;

 private:
  void BeforeRender();

  void OnRenderScene(float ds0);

  void HandleShortcuts();

  void OpenProject();

  void NewScene();

  void SaveScene();

  void SaveSceneAs();

  // Open scene with an modal
  void OpenScene();

  // Load scene from path
  void OpenScene(const fs::path& path);

  // Open scene from scene manager
  void OpenScene(const uint32_t index);

  // Toolbar button callbacks
  void OnScenePlay();

  void OnSceneStop();

  void OnScenePause();

  void OnSceneResume();

  void OnSceneStep();

  void OnSceneEject();

  void SetSceneState(SceneState state);

  void Exit(bool force = false);

  void OnSceneModify();

  void OnSceneSave();

  void SetSceneTitle();

  void OnExitModalAnswer(ExitModalAnswer answer);

  void SetupMenubar();

 private:
  // rendering
  Ref<FrameBuffer> frame_buffer_;
  Scope<SceneRenderer> scene_renderer_;

  EditorCamera editor_camera_;
  bool camera_translatable_;

  // scene members
  Ref<Scene> editor_scene_;
  SceneState scene_state_ = SceneState::kEdit;
  fs::path editor_scene_path_;

  // widgets
  ExitModal exit_modal_;
  MenuBar menu_bar_;

  // panels
  Scope<ConsolePanel> console_panel_;
  Scope<DebugInfoPanel> debug_info_panel_;
  Ref<HierarchyPanel> hierarchy_panel_;
  Scope<InspectorPanel> inspector_panel_;
  Scope<ToolbarPanel> toolbar_panel_;
  Scope<ViewportPanel> viewport_panel_;

  // is camera ejected
  bool is_ejected_;

  bool unsaved_changes_{false};
  bool show_exit_dialog_{false};

  CursorState old_cursor_state_ = CursorState::kNormal;
};
}  // namespace eve
