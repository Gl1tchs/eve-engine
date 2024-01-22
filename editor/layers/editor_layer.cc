// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "layers/editor_layer.h"

#include <ImGuizmo.h>
#include <tinyfiledialogs.h>

#include "core/event/event_handler.h"
#include "core/event/input.h"
#include "core/event/key_code.h"
#include "core/event/mouse_code.h"
#include "core/event/window_event.h"
#include "core/utils/memory.h"
#include "graphics/render_command.h"
#include "graphics/scene_renderer.h"
#include "project/project.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/scene_serializer.h"
#include "scripting/script_engine.h"

#include "utils/modify_info.h"
#include "widgets/dock_space.h"

namespace eve {

EditorLayer::EditorLayer(Ref<State>& state) : Layer(state) {
  // Setup delegates
  { exit_modal_.on_answer = BIND_FUNC(OnExitModalAnswer); }
}

void EditorLayer::OnCreate() {
  // Remove default beheaviour
  PopEvent<WindowCloseEvent>();
  SubscribeEvent<WindowCloseEvent>(
      [this](const WindowCloseEvent& event) { Exit(); });

  frame_buffer_ = FrameBuffer::Create({300, 300});

  editor_camera_ = CreateRef<EditorCamera>();

  hierarchy_panel_ = CreateRef<HierarchyPanel>();

  viewport_panel_ = CreateScope<ViewportPanel>(frame_buffer_, hierarchy_panel_,
                                               editor_camera_);
  {
    viewport_panel_->on_play = BIND_FUNC(OnScenePlay);
    viewport_panel_->on_stop = BIND_FUNC(OnSceneStop);
    viewport_panel_->on_pause = BIND_FUNC(OnScenePause);
    viewport_panel_->on_resume = BIND_FUNC(OnSceneResume);
    viewport_panel_->on_step = BIND_FUNC(OnSceneStep);
  }

  inspector_panel_ = CreateScope<InspectorPanel>(hierarchy_panel_);

  debug_info_panel_ = CreateScope<DebugInfoPanel>(GetState()->renderer);

  content_browser_ = CreateScope<ContentBrowserPanel>();
  content_browser_->on_scene_open = BIND_FUNC(OpenScene);

  // If scene alredy loaded register it.
  if (auto& active_scene = SceneManager::GetActive(); active_scene) {
    SetSceneTitle();

    editor_scene_ = SceneManager::GetActive();
    editor_scene_path_ = SceneManager::GetActivePath();

    content_browser_->Reload();
  }

  scene_renderer_ = CreateRef<SceneRenderer>(GetState());

  scene_settings_panel_ =
      CreateRef<SceneSettingsPanel>(scene_renderer_, editor_camera_);

  SetupMenubar();
}

void EditorLayer::OnDestroy() {}

void EditorLayer::OnUpdate(float ds) {
  if (!SceneManager::GetActive()) {
    HandleShortcuts();
    return;
  }

  BeforeRender();
  OnRenderScene(ds);

  if (SceneManager::GetActive()->IsRunning()) {
    if (Input::IsKeyPressed(KeyCode::kEscape)) {
      SetCursorMode(CursorMode::kNormal);
    }
  }
}

void EditorLayer::OnGUI(float ds) {
  RenderCommand::Clear(BufferBits_kColor);

  DockSpace::Begin();
  {
    menu_bar_.Draw();

    if (!SceneManager::GetActive()) {
      DockSpace::End();
      return;
    }

    // Render panels
    console_panel_.Render();
    content_browser_->Render();
    asset_registry_panel_.Render();
    project_settings_panel_.Render();
    about_panel_.Render();

    viewport_panel_->Render();
    hierarchy_panel_->Render();
    inspector_panel_->Render();
    scene_settings_panel_->Render();
    debug_info_panel_->Render();

    // Render modals
    exit_modal_.Render();
  }
  DockSpace::End();
}

void EditorLayer::BeforeRender() {
  if (modify_info.modified && !unsaved_changes_) {
    OnSceneModify();
  }

  auto& viewport_size = viewport_panel_->GetSize();
  scene_renderer_->OnViewportResize(
      {(uint32_t)viewport_size.x, (uint32_t)viewport_size.y});

  // Resize
  if (glm::vec2 size = frame_buffer_->GetSize();
      viewport_size.x > 0.0f &&
      viewport_size.y > 0.0f &&  // zero sized framebuffer is invalid
      (size.x != viewport_size.x || size.y != viewport_size.y)) {
    frame_buffer_->SetSize(
        {(uint32_t)viewport_size.x, (uint32_t)viewport_size.y});
    editor_camera_->aspect_ratio = viewport_size.x / viewport_size.y;
  }
}

void EditorLayer::OnRenderScene(float ds) {
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
    case SceneState::kEdit: {
      viewport_panel_->SetShouldDrawGizmos(true);

      // Update camera movement
      if (viewport_panel_->IsFocused() &&
          Input::IsMouseButtonPressed(MouseCode::kRight)) {
        camera_translatable_ = true;

        // hide the cursor since we are now controlling the camera
        if (cursor_mode_ == CursorMode::kNormal) {
          SetCursorMode(CursorMode::kDisabled);
        }
        editor_camera_->Update(ds);
      } else {
        camera_translatable_ = false;

        // set cursor to normal again
        if (cursor_mode_ != CursorMode::kNormal) {
          SetCursorMode(CursorMode::kNormal);
        }
        editor_camera_->ResetMousePos();
      }

      if (auto& scene = SceneManager::GetActive(); scene) {
        scene->OnUpdateEditor(ds);
        scene_renderer_->RenderEditor(ds, editor_camera_);
      }

      HandleShortcuts();

      break;
    }
    case SceneState::kPaused:
    case SceneState::kPlay: {
      viewport_panel_->SetShouldDrawGizmos(false);

      if (auto& scene = SceneManager::GetActive(); scene) {
        scene->OnUpdateRuntime(ds);
        scene_renderer_->RenderRuntime(ds);
      }

      break;
    }
  }

  frame_buffer_->Unbind();
}

void EditorLayer::HandleShortcuts() {
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantTextInput) {
    return;
  }

  // Shortcuts
  if (Input::IsKeyPressed(KeyCode::kLeftControl)) {
    if (SceneManager::GetActive()) {
      if (Input::IsKeyPressed(KeyCode::kO)) {
        OpenScene();
      }

      if (Input::IsKeyPressed(KeyCode::kN)) {
        NewScene();
      }

      if (Input::IsKeyPressed(KeyCode::kS)) {
        SaveScene();
      }

      if (Input::IsKeyPressed(KeyCode::kR)) {
        ScriptEngine::ReloadAssembly();
      }

      if (Input::IsKeyPressed(KeyCode::kLeftShift)) {
        if (Input::IsKeyPressed(KeyCode::kO)) {
          OpenProject();
        }

        if (Input::IsKeyPressed(KeyCode::kS)) {
          SaveSceneAs();
        }
      }
    } else {
      if (Input::IsKeyPressed(KeyCode::kO)) {
        OpenProject();
      }
    }

    if (Input::IsKeyPressed(KeyCode::kLeftShift)) {
      if (Input::IsKeyPressed(KeyCode::kQ)) {
        Exit();
      }
    }
  }

  // Gizmo and transform manuplation shortcuts
  if (!camera_translatable_ && !ImGuizmo::IsUsing()) {
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
      SceneManager::GetActive()->DestroyEntity(entity);
      hierarchy_panel_->SetSelectedEntity(kInvalidEntity);
      modify_info.SetModified();
    }
  }
}

void EditorLayer::OpenProject() {
  const char* filter_patterns[1] = {"*.eproj"};
  const char* path = tinyfd_openFileDialog(
      "Open Project", "", 1, filter_patterns, "Eve Project Files", 0);

  if (!path) {
    EVE_LOG_EDITOR_ERROR("Unable to open project from path.");
    return;
  }

  if (Ref<Project> project = Project::Load(fs::path(path)); project) {
    if (!ScriptEngine::IsInitialized()) {
      ScriptEngine::Init();
    } else {
      hierarchy_panel_->SetSelectedEntity(kInvalidEntity);
      ScriptEngine::Reinit();
    }

    // Open the first scene index
    if (SceneManager::DoesIndexExists(0)) {
      OpenScene(0);
    } else {
      NewScene();
    }

    content_browser_->Reload();
  }
}

void EditorLayer::NewScene() {
  SceneManager::GetActive() = CreateRef<Scene>(GetState(), "untitled");

  editor_scene_path_ = "";

  modify_info.SetModified();
  editor_camera_->ResetTransform();
}

void EditorLayer::SaveScene() {
  if (!editor_scene_path_.empty()) {
    SceneSerializer serializer(SceneManager::GetActive());
    serializer.Serialize(editor_scene_path_);

    OnSceneSave();
  } else {
    SaveSceneAs();
  }
}

void EditorLayer::SaveSceneAs() {
  const char* filter_patterns[1] = {"*.escn"};
  const char* path = tinyfd_saveFileDialog("Save Scene", "scene.escn", 1,
                                           filter_patterns, "Eve Scene Files");

  if (!path) {
    EVE_LOG_EDITOR_ERROR("Unable to save scene to path.");
    return;
  }

  editor_scene_path_ = std::string(path);

  SceneSerializer serializer(SceneManager::GetActive());
  serializer.Serialize(editor_scene_path_);

  OnSceneSave();
}

void EditorLayer::OpenScene() {
  if (scene_state_ != SceneState::kEdit) {
    return;
  }

  const char* filter_patterns[1] = {"*.escn"};
  const char* path = tinyfd_openFileDialog("Open Scene", "", 1, filter_patterns,
                                           "Eve Scene Files", 0);

  if (!path) {
    EVE_LOG_EDITOR_ERROR("Unable to open scene from path.");
    return;
  }

  OpenScene(path);
}

void EditorLayer::OpenScene(const fs::path& path) {
  if (scene_state_ != SceneState::kEdit) {
    return;
  }

  SetSceneTitle();

  Ref<Scene> new_scene = CreateRef<Scene>(GetState());
  SceneSerializer serializer(new_scene);
  if (serializer.Deserialize(path)) {
    editor_scene_ = new_scene;
    SceneManager::GetActive() = editor_scene_;

    editor_scene_path_ = path;

    editor_camera_->ResetTransform();
    hierarchy_panel_->selected_entity_ = kInvalidEntity;
  }
}

void EditorLayer::OpenScene(const uint32_t index) {
  if (scene_state_ != SceneState::kEdit) {
    return;
  }

  if (index >= SceneManager::GetRegisteredSceneCount()) {
    return;
  }

  SetSceneTitle();

  SceneManager::SetActive(index);

  editor_scene_ = SceneManager::GetActive();
  editor_scene_path_ = SceneManager::GetActivePath();

  editor_camera_->ResetTransform();
}

void EditorLayer::OnScenePlay() {
  SetSceneState(SceneState::kPlay);

  SceneManager::GetActive() = Scene::Copy(editor_scene_);

  // if we encounter with an error stop the scene
  if (!SceneManager::GetActive()->OnRuntimeStart()) {
    SetSceneState(SceneState::kEdit);

    return;
  }
}

void EditorLayer::OnSceneStop() {
  SetSceneState(SceneState::kEdit);

  SceneManager::GetActive()->OnRuntimeStop();
  SceneManager::GetActive() = editor_scene_;
}

void EditorLayer::OnScenePause() {
  SetSceneState(SceneState::kPaused);
  SceneManager::GetActive()->SetPaused(true);
}

void EditorLayer::OnSceneResume() {
  SetSceneState(SceneState::kPlay);
  SceneManager::GetActive()->SetPaused(false);
}

void EditorLayer::OnSceneStep() {
  SceneManager::GetActive()->Step();
}

void EditorLayer::SetSceneState(SceneState state) {
  scene_state_ = state;
  viewport_panel_->SetState(state);
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
  if (auto project = Project::GetActive(); project) {
    project->SaveActive(project->GetProjectPath());
  }

  SetSceneTitle();

  modify_info.OnSave();
  unsaved_changes_ = false;
  exit_modal_.SetActive(false);
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
      exit_modal_.SetActive(false);
      break;
    default:
      break;
  }
}

void EditorLayer::SetupMenubar() {
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
      MenuItem open_scene(
          "Open Scene", [&]() { OpenScene(); }, "Ctrl+O");
      scene_group.PushMenuItem(open_scene);

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
      MenuItem project_settings("Project Settings", [this]() {
        project_settings_panel_.SetActive(true);
      });
      base_group.PushMenuItem(project_settings);

      MenuItem scene_settings("Scene Settings", [this]() {
        scene_settings_panel_->SetActive(true);
      });
      base_group.PushMenuItem(scene_settings);

      MenuItem reload_scene("Reload Scene",
                            [this]() { OpenScene(editor_scene_path_); });
      base_group.PushMenuItem(reload_scene);

      MenuItem reload_scripts(
          "Reload Scripts", [this]() { ScriptEngine::ReloadAssembly(); },
          "Ctrl+R");
      base_group.PushMenuItem(reload_scripts);

      edit_menu.PushItemGroup(base_group);
    }

    MenuItemGroup renderer_group;
    {
      MenuItem recompile_shaders("Recompile Shaders", [this]() {
        GetState()->renderer->RecompileShaders();
      });
      renderer_group.PushMenuItem(recompile_shaders);

      edit_menu.PushItemGroup(renderer_group);
    }

    menu_bar_.PushMenu(edit_menu);
  }

  Menu view_menu("View");
  {
    MenuItemGroup base_group;
    {
      MenuItem viewport("Viewport",
                        [this]() { viewport_panel_->SetActive(true); });
      base_group.PushMenuItem(viewport);

      MenuItem hierarchy("Hierarchy",
                         [this]() { hierarchy_panel_->SetActive(true); });
      base_group.PushMenuItem(hierarchy);

      MenuItem inspector("Inspector",
                         [this]() { inspector_panel_->SetActive(true); });
      base_group.PushMenuItem(inspector);

      MenuItem console("Console", [this]() { console_panel_.SetActive(true); });
      base_group.PushMenuItem(console);

      MenuItem content_browser("Content Browser",
                               [this]() { content_browser_->SetActive(true); });
      base_group.PushMenuItem(content_browser);

      MenuItem asset_registry_panel("Asset Registry", [this]() {
        asset_registry_panel_.SetActive(true);
      });
      base_group.PushMenuItem(asset_registry_panel);

      MenuItem advanced_inspector("Advanced Inspector", [this]() {
        inspector_panel_->ToggleAdvanced();
      });
      base_group.PushMenuItem(advanced_inspector);

      view_menu.PushItemGroup(base_group);
    }

    MenuItemGroup renderer_group;
    {
      MenuItem debug_info("Debug Info",
                          [this]() { debug_info_panel_->SetActive(true); });
      renderer_group.PushMenuItem(debug_info);

      view_menu.PushItemGroup(renderer_group);
    }

    menu_bar_.PushMenu(view_menu);
  }

  Menu help_menu("Help");
  {
    MenuItemGroup base_group;
    {
      MenuItem inspector("About", [this]() { about_panel_.SetActive(true); });
      base_group.PushMenuItem(inspector);

      help_menu.PushItemGroup(base_group);
    }

    menu_bar_.PushMenu(help_menu);
  }
}

void EditorLayer::SetCursorMode(CursorMode mode) {
  cursor_mode_ = mode;
  GetState()->window->SetCursorMode(cursor_mode_);
}

}  // namespace eve
