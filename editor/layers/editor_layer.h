// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/debug/log.h"
#include "core/layer.h"
#include "graphics/frame_buffer.h"
#include "scene/editor_camera.h"
#include "scene/scene.h"

#include "modals/exit_modal.h"
#include "panels/hierarchy_panel.h"
#include "panels/inspector_panel.h"
#include "panels/render_stats_panel.h"
#include "panels/toolbar_panel.h"
#include "panels/viewport_panel.h"
#include "widgets/menu_bar.h"

class EditorLayer : public Layer {
 public:
  EditorLayer(Ref<State>& state);
  ~EditorLayer();

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

  void OpenScene(const std::filesystem::path& path);

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

 private:
  Ref<LogInstance> editor_logger_;

  Ref<Scene> active_scene_;
  Ref<Scene> editor_scene_;
  SceneState scene_state_ = SceneState::kEdit;
  std::string editor_scene_path_;

  EditorCamera editor_camera_;
  bool camera_translatable_;

  Ref<FrameBuffer> frame_buffer_;

  Scope<ToolbarPanel> toolbar_panel_;
  bool is_ejected_;

  Scope<ViewportPanel> viewport_panel_;
  Ref<HierarchyPanel> hierarchy_panel_;
  Scope<InspectorPanel> inspector_panel_;
  Scope<RenderStatsPanel> render_stats_panel_;

  ExitModal exit_modal_;

  MenuBar menu_bar_;

  bool unsaved_changes_{false};
  bool show_exit_dialog_{false};

  CursorState old_cursor_state_ = CursorState::kNormal;
};
