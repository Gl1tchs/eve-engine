// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "layers/editor_layer.h"

#include <tinyfiledialogs.h>

#include "core/debug/instrumentor.h"
#include "core/event/event_handler.h"
#include "core/event/events.h"
#include "core/event/input.h"
#include "core/utils/memory.h"
#include "graphics/render_command.h"
#include "project/project.h"
#include "scene/scene_serializer.h"
#include "utils/modify_info.h"
#include "widgets/dock_space.h"

EditorLayer::EditorLayer(Ref<State>& state)
    : Layer(state), exit_modal_(BIND_FUNC(OnExitModalAnswer)) {
  PROFILE_FUNCTION();

  editor_logger_ = LoggerManager::AddLogger("EDITOR");

  // Remove default beheaviour
  PopEvent<WindowCloseEvent>();
  SubscribeEvent<WindowCloseEvent>(
      [this](const WindowCloseEvent& event) { Exit(); });
}

EditorLayer::~EditorLayer() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnStart() {
  PROFILE_FUNCTION();

  editor_scene_ = CreateRef<Scene>(GetState());
  active_scene_ = editor_scene_;

  frame_buffer_ = FrameBuffer::Create({300, 300});

  viewport_panel_ = CreateScope<ViewportPanel>(frame_buffer_);
  hierarchy_panel_ = CreateRef<HierarchyPanel>();
  inspector_panel_ = CreateScope<InspectorPanel>(hierarchy_panel_);
  render_stats_panel_ = CreateScope<RenderStatsPanel>(GetState()->renderer);

  Menu file_menu("File");
  {
    MenuItemGroup project_group;
    {
      MenuItem open_project(
          "Open Project", [&]() { OpenProject(); }, "Ctrl+O");
      project_group.PushMenuItem(open_project);

      file_menu.PushItemGroup(project_group);
    }

    MenuItemGroup scene_group(
        []() -> bool { return Project::GetActive() != nullptr; });
    {
      MenuItem new_scene(
          "New Scene", [&]() { NewScene(); }, "Ctrl+N");
      scene_group.PushMenuItem(new_scene);

      MenuItem save_scene(
          "Save Scene", [&]() { SaveScene(); }, "Ctrl+S");
      scene_group.PushMenuItem(save_scene);

      MenuItem save_scene_as(
          "Save Scene As", [&]() { SaveSceneAs(); }, "Ctrl+Shift+S");
      scene_group.PushMenuItem(save_scene_as);

      file_menu.PushItemGroup(scene_group);
    }

    MenuItemGroup system_group;
    {
      MenuItem exit(
          "Exit", [&]() { Exit(); }, "Ctrl+Shift+Q");
      system_group.PushMenuItem(exit);

      file_menu.PushItemGroup(system_group);
    }

    menu_bar_.PushMenu(file_menu);
  }

  Menu view_menu("View");
  {
    MenuItemGroup base_group;
    {
      MenuItem hierarchy("Hierarchy",
                         [this]() { hierarchy_panel_->SetActive(true); });
      base_group.PushMenuItem(hierarchy);

      MenuItem inspector("Inspector",
                         [this]() { inspector_panel_->SetActive(true); });
      base_group.PushMenuItem(inspector);

      view_menu.PushItemGroup(base_group);
    }

    MenuItemGroup renderer_group;
    {
      MenuItem render_stats("Render Stats",
                            [this]() { render_stats_panel_->SetActive(true); });
      renderer_group.PushMenuItem(render_stats);

      view_menu.PushItemGroup(renderer_group);
    }

    menu_bar_.PushMenu(view_menu);
  }
}

void EditorLayer::OnDestroy() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(float ds) {
  PROFILE_FUNCTION();

  if (modify_info.modified && !unsaved_changes_) {
    OnSceneModify();
  }

  auto& viewport_size = viewport_panel_->GetSize();

  active_scene_->OnViewportResize(
      {(uint32_t)viewport_size.x, (uint32_t)viewport_size.y});

  // Resize
  if (glm::vec2 size = frame_buffer_->GetSize();
      viewport_size.x > 0.0f &&
      viewport_size.y > 0.0f &&  // zero sized framebuffer is invalid
      (size.x != viewport_size.x || size.y != viewport_size.y)) {
    frame_buffer_->SetSize(
        {(uint32_t)viewport_size.x, (uint32_t)viewport_size.y});
    editor_camera_.SetAspectRatio(viewport_size.x / viewport_size.y);
  }

  Ref<Renderer>& renderer = GetState()->renderer;

  renderer->ResetStats();

  frame_buffer_->Bind();
  frame_buffer_->Refresh();

  RenderCommand::SetViewport(0, 0, (uint32_t)viewport_size.x,
                             (uint32_t)viewport_size.y);
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
  RenderCommand::Clear(BufferBits_kColor | BufferBits_kDepth);

  switch (scene_state_) {
    case SceneState::kEdit: {
      if (viewport_panel_->IsFocused() &&
          Input::IsMouseButtonPressed(MouseCode::kRight)) {
        // hide the cursor since we are now controlling the camera
        if (old_cursor_state_ == CursorState::kNormal) {
          old_cursor_state_ = CursorState::kHidden;
          GetState()->window->SetCursorState(old_cursor_state_);
        }
        editor_camera_.Update(ds);
      } else {
        // set cursor to normal again
        if (old_cursor_state_ == CursorState::kHidden) {
          old_cursor_state_ = CursorState::kNormal;
          GetState()->window->SetCursorState(old_cursor_state_);
        }
        editor_camera_.ResetMousePos();
      }

      active_scene_->OnUpdateEditor(ds, editor_camera_);
      break;
    }
    case SceneState::kPlay: {
      active_scene_->OnUpdateRuntime(ds);
      break;
    }
  }

  frame_buffer_->Unbind();

  // Shortcuts
  if (Input::IsKeyPressed(KeyCode::kLeftControl)) {
    if (Input::IsKeyPressed(KeyCode::kO)) {
      OpenProject();
    }

    if (Input::IsKeyPressed(KeyCode::kN)) {
      NewScene();
    }

    if (Input::IsKeyPressed(KeyCode::kS)) {
      SaveScene();
    }

    if (Input::IsKeyPressed(KeyCode::kLeftShift)) {
      if (Input::IsKeyPressed(KeyCode::kS)) {
        SaveSceneAs();
      }

      if (Input::IsKeyPressed(KeyCode::kQ)) {
        Exit();
      }
    }
  }

  if (auto entity = hierarchy_panel_->GetSelectedEntity(); entity) {
    if (Input::IsKeyPressed(KeyCode::kDelete)) {
      active_scene_->DestroyEntity(entity);
      hierarchy_panel_->SetSelectedEntity(Entity{});
      modify_info.SetModified();
    }
  }
}

void EditorLayer::OnGUI(float ds) {
  PROFILE_FUNCTION();

  DockSpace::Begin();
  {
    menu_bar_.Draw();

    // Render panels
    viewport_panel_->Render();
    hierarchy_panel_->Render();
    inspector_panel_->Render();
    render_stats_panel_->Render();

    exit_modal_.Render();
  }
  DockSpace::End();
}

void EditorLayer::OpenProject() {
  const char* filter_patterns[1] = {"*.eproject"};
  const char* path = tinyfd_openFileDialog("Open Scene", "", 1, filter_patterns,
                                           "Eve Project Files", 0);

  if (!path) {
    LOG_ENGINE_ERROR("Unable to open scene from path.");
    return;
  }

  if (Ref<Project> project = Project::Load(std::string(path)); project) {
    auto& project_config = project->GetConfig();

    SetSceneTitle();

    OpenScene(
        AssetLibrary::GetAssetPath(project_config.default_scene.string()));
  }
}

void EditorLayer::NewScene() {
  active_scene_ = CreateRef<Scene>(GetState(), "untitled");
  hierarchy_panel_->SetScene(active_scene_);

  editor_scene_path_ = "";

  modify_info.SetModified();
  editor_camera_.ResetTransform();
}

void EditorLayer::SaveScene() {
  if (!editor_scene_path_.empty()) {
    SceneSerializer serializer(active_scene_);
    serializer.Serialize(editor_scene_path_);

    OnSceneSave();
  } else {
    SaveSceneAs();
  }
}

void EditorLayer::SaveSceneAs() {
  const char* filter_patterns[1] = {"*.eve"};
  const char* path = tinyfd_saveFileDialog("Save Scene", "scene.eve", 1,
                                           filter_patterns, "Eve Scene Files");

  if (!path) {
    LOG_ENGINE_ERROR("Unable to save scene to path.");
    return;
  }

  editor_scene_path_ = std::string(path);

  SceneSerializer serializer(active_scene_);
  serializer.Serialize(editor_scene_path_);

  OnSceneSave();
}

void EditorLayer::OpenScene(const std::filesystem::path& path) {
  if (scene_state_ != SceneState::kEdit) {
    OnScenePause();
  }

  Ref<Scene> new_scene = CreateRef<Scene>(GetState());
  SceneSerializer serializer(new_scene);
  if (serializer.Deserialize(path)) {
    editor_scene_ = new_scene;
    active_scene_ = editor_scene_;

    hierarchy_panel_->SetScene(active_scene_);

    editor_scene_path_ = path.string();

    editor_camera_.ResetTransform();
  }
}

void EditorLayer::OnScenePlay() {
  scene_state_ = SceneState::kPlay;

  active_scene_ = Scene::Copy(editor_scene_);
  active_scene_->OnRuntimeStart();

  hierarchy_panel_->SetScene(active_scene_);
}

void EditorLayer::OnSceneStop() {
  if (scene_state_ == SceneState::kPlay) {
    active_scene_->OnRuntimeStop();
  }

  scene_state_ = SceneState::kEdit;

  active_scene_ = editor_scene_;

  hierarchy_panel_->SetScene(active_scene_);
}

void EditorLayer::OnScenePause() {
  if (scene_state_ == SceneState::kEdit)
    return;

  active_scene_->SetPaused(true);
}

void EditorLayer::Exit(bool force) {
  if (modify_info.modified && !force) {
    exit_modal_.SetActive(true);
    return;
  }

  GetState()->running = false;
}

void EditorLayer::OnSceneModify() {
  auto window = GetState()->window;
  std::string title = window->GetTitle();
  window->SetTitle(title + " *");

  unsaved_changes_ = true;
}

void EditorLayer::OnSceneSave() {
  SetSceneTitle();

  modify_info.OnSave();
  unsaved_changes_ = false;
  show_exit_dialog_ = false;
}

void EditorLayer::SetSceneTitle() {
  GetState()->window->SetTitle(
      std::format("Eve Editor | {0}", Project::GetActive()->GetConfig().name));
}

void EditorLayer::OnExitModalAnswer(ExitModalAnswer answer) {
  switch (answer) {
    case ExitModalAnswer::kSaveAndExit:
      SaveScene();
      Exit();
      break;
    case ExitModalAnswer::kExitWithoutSaving:
      Exit(true);
      break;
    case ExitModalAnswer::kCancel:
      show_exit_dialog_ = false;
      break;
    default:
      break;
  }
}
