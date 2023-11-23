// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "layers/editor_layer.h"

#include <ImGuizmo.h>
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

EditorLayer::EditorLayer(Ref<State>& state) : Layer(state) {
  PROFILE_FUNCTION();

  editor_logger_ = LoggerManager::AddLogger("EDITOR");

  exit_modal_.on_answer = BIND_FUNC(OnExitModalAnswer);

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

  frame_buffer_ = FrameBuffer::Create({300, 300});

  toolbar_panel_ = CreateScope<ToolbarPanel>();
  {
    toolbar_panel_->on_play = BIND_FUNC(OnScenePlay);
    toolbar_panel_->on_stop = BIND_FUNC(OnSceneStop);
    toolbar_panel_->on_pause = BIND_FUNC(OnScenePause);
    toolbar_panel_->on_resume = BIND_FUNC(OnSceneResume);
    toolbar_panel_->on_step = BIND_FUNC(OnSceneStep);
    toolbar_panel_->on_eject = BIND_FUNC(OnSceneEject);
  }

  hierarchy_panel_ = CreateRef<HierarchyPanel>();
  viewport_panel_ = CreateScope<ViewportPanel>(frame_buffer_, hierarchy_panel_,
                                               &editor_camera_);
  inspector_panel_ = CreateScope<InspectorPanel>(hierarchy_panel_);
  render_stats_panel_ = CreateScope<RenderStatsPanel>(GetState()->renderer);

  // If active project run it.
  if (Ref<Project> project = Project::GetActive(); project) {
    auto& project_config = project->GetConfig();

    SetSceneTitle();

    OpenScene(
        AssetLibrary::GetAssetPath(project_config.default_scene.string()));
  }

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

  Menu edit_menu("Edit");
  {
    MenuItemGroup base_group;
    {
      MenuItem advanced_inspector("Advanced Inspector", [this]() {
        inspector_panel_->ToggleAdvanced();
      });

      base_group.PushMenuItem(advanced_inspector);

      edit_menu.PushItemGroup(base_group);
    }

    menu_bar_.PushMenu(edit_menu);
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

  BeforeRender();
  OnRenderScene(ds);
}

void EditorLayer::OnGUI(float ds) {
  PROFILE_FUNCTION();

  DockSpace::Begin();
  {
    menu_bar_.Draw();

    if (!active_scene_) {
      DockSpace::End();
      return;
    }

    // Render panels
    toolbar_panel_->Render();
    viewport_panel_->Render();
    hierarchy_panel_->Render();
    inspector_panel_->Render();
    render_stats_panel_->Render();

    // Render modals
    exit_modal_.Render();
  }
  DockSpace::End();
}

void EditorLayer::BeforeRender() {
  if (!active_scene_) {
    return;
  }

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
    editor_camera_.aspect_ratio = viewport_size.x / viewport_size.y;
  }
}

void EditorLayer::OnRenderScene(float ds) {
  if (!active_scene_) {
    return;
  }

  Ref<Renderer>& renderer = GetState()->renderer;

  renderer->ResetStats();

  frame_buffer_->Bind();
  frame_buffer_->Refresh();

  auto& viewport_size = viewport_panel_->GetSize();
  RenderCommand::SetViewport(0, 0, (uint32_t)viewport_size.x,
                             (uint32_t)viewport_size.y);
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
  RenderCommand::Clear(BufferBits_kColor | BufferBits_kDepth);

  switch (scene_state_) {
    case SceneState::kPaused:
    case SceneState::kEdit: {
      // Update camera movement
      if (viewport_panel_->IsFocused() &&
          Input::IsMouseButtonPressed(MouseCode::kRight)) {
        camera_translatable_ = true;

        // hide the cursor since we are now controlling the camera
        if (old_cursor_state_ == CursorState::kNormal) {
          old_cursor_state_ = CursorState::kHidden;
          GetState()->window->SetCursorState(old_cursor_state_);
        }
        editor_camera_.Update(ds);
      } else {
        camera_translatable_ = false;

        // set cursor to normal again
        if (old_cursor_state_ == CursorState::kHidden) {
          old_cursor_state_ = CursorState::kNormal;
          GetState()->window->SetCursorState(old_cursor_state_);
        }
        editor_camera_.ResetMousePos();
      }

      active_scene_->OnUpdateEditor(ds, editor_camera_, !is_ejected_);

      HandleShortcuts();

      break;
    }
    case SceneState::kPlay: {
      active_scene_->OnUpdateRuntime(ds);
      break;
    }
  }

  frame_buffer_->Unbind();
}

void EditorLayer::HandleShortcuts() {
  if (camera_translatable_) {
    return;
  }

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

  // Gizmo and transform manuplation shortcuts
  if (!ImGuizmo::IsUsing()) {
    if (Input::IsKeyPressed(KeyCode::kQ)) {
      viewport_panel_->SetGizmoState(-1);
    }

    if (Input::IsKeyPressed(KeyCode::kW)) {
      viewport_panel_->SetGizmoState(ImGuizmo::OPERATION::TRANSLATE);
    }

    if (Input::IsKeyPressed(KeyCode::kE)) {
      viewport_panel_->SetGizmoState(ImGuizmo::OPERATION::ROTATE);
    }

    if (Input::IsKeyPressed(KeyCode::kR)) {
      viewport_panel_->SetGizmoState(ImGuizmo::OPERATION::SCALE);
    }

    if (Input::IsKeyPressed(KeyCode::kT)) {
      viewport_panel_->SetGizmoState(ImGuizmo::OPERATION::UNIVERSAL);
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
  SetSceneState(SceneState::kPlay);

  active_scene_ = Scene::Copy(editor_scene_);
  active_scene_->OnRuntimeStart();

  hierarchy_panel_->SetScene(active_scene_);

  is_ejected_ = false;
}

void EditorLayer::OnSceneStop() {
  SetSceneState(SceneState::kEdit);

  active_scene_ = editor_scene_;
  hierarchy_panel_->SetScene(active_scene_);

  is_ejected_ = true;
}

void EditorLayer::OnScenePause() {
  SetSceneState(SceneState::kPaused);
  active_scene_->SetPaused(true);

  is_ejected_ = false;
}

void EditorLayer::OnSceneResume() {
  SetSceneState(SceneState::kPlay);
  active_scene_->SetPaused(false);

  is_ejected_ = false;
}

void EditorLayer::OnSceneStep() {
  active_scene_->Step();
}

void EditorLayer::OnSceneEject() {
  is_ejected_ = !is_ejected_;
}

void EditorLayer::SetSceneState(SceneState state) {
  scene_state_ = state;
  toolbar_panel_->SetState(state);
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
