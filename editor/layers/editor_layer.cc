// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "layers/editor_layer.h"

#include <imgui.h>
#include <tinyfiledialogs.h>

#include "core/debug/instrumentor.h"
#include "core/event/input.h"
#include "editor_layer.h"
#include "graphics/render_command.h"
#include "project/project.h"
#include "scene/entity.h"
#include "scene/scene_serializer.h"

EditorLayer::EditorLayer(Ref<State>& state) : Layer(state) {
  PROFILE_FUNCTION();

  editor_logger_ = LoggerManager::AddLogger("EDITOR");
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
}

void EditorLayer::OnDestroy() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(float ds) {
  PROFILE_FUNCTION();

  if (inspector_panel_->IsModified() && !modified_title_updated_) {
    OnSceneModify();
  }

  auto viewport_size = viewport_panel_->GetSize();

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
      editor_camera_.Update(ds);

      active_scene_->OnUpdateEditor(ds, editor_camera_);
      break;
    }
    case SceneState::kPlay: {
      active_scene_->OnUpdateRuntime(ds);
      break;
    }
  }

  frame_buffer_->Unbind();
}

void EditorLayer::OnGUI(float ds) {
  PROFILE_FUNCTION();

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

  BeginDockspace();
  {
    DrawMenubar();
    // Render panels
    viewport_panel_->Render();
    hierarchy_panel_->Render();
    inspector_panel_->Render();
    render_stats_panel_->Render();
  }
  EndDockspace();
}

void EditorLayer::BeginDockspace() {
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("MyDockSpace", nullptr, window_flags);
  ImGui::PopStyleVar(3);

  // DockSpace
  ImGuiIO& io = ImGui::GetIO();
  ImGuiStyle& style = ImGui::GetStyle();
  float min_win_size_x = style.WindowMinSize.x;
  style.WindowMinSize.x = 370.0f;
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  style.WindowMinSize.x = min_win_size_x;
}

void EditorLayer::EndDockspace() {
  ImGui::End();
}

void EditorLayer::DrawMenubar() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
        OpenProject();
      }

      ImGui::Separator();

      if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
        NewScene();
      }

      if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
        SaveScene();
      }

      if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S")) {
        SaveSceneAs();
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Exit", "Ctrl+Shift+Q")) {
        Exit();
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Render Stats")) {
        render_stats_panel_->SetActive(true);
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }
}

void EditorLayer::OpenProject() {
  const char* filter_patterns[1] = {"*.eproject"};
  const char* path = tinyfd_openFileDialog("Open Scene", "", 1, filter_patterns,
                                           "Eve Project Files", 0);

  if (!path) {
    LOG_ENGINE_ERROR("Unable to open scene from path: {0}\n", path);
    return;
  }

  if (Ref<Project> project = Project::Load(path); project) {
    // ScriptEngine::Init();

    auto& project_config = project->GetConfig();

    SetSceneTitle();

    OpenScene(
        AssetLibrary::GetAssetPath(project_config.default_scene.string()));
  }
}

void EditorLayer::NewScene() {
  editor_scene_path_ = "";
  scene_state_ = SceneState::kEdit;

  active_scene_ = CreateRef<Scene>(GetState(), "untitled");
}

void EditorLayer::SaveScene() {
  if (!editor_scene_path_.empty()) {
    SceneSerializer serializer(active_scene_, GetState()->asset_library);
    serializer.Serialize(editor_scene_path_);

    OnSceneSave();
  } else {
    SaveSceneAs();
  }
}

void EditorLayer::SaveSceneAs() {
  const char* filter_patterns[1] = {"*.eve"};
  std::string path = tinyfd_saveFileDialog("Save Scene", "scene.eve", 1,
                                           filter_patterns, "Eve Scene Files");

  if (path.empty()) {
    LOG_ENGINE_ERROR("Unable to save scene to path: {0}\n", path);
    return;
  }

  editor_scene_path_ = path;

  SceneSerializer serializer(active_scene_, GetState()->asset_library);
  serializer.Serialize(editor_scene_path_);

  OnSceneSave();
}

void EditorLayer::OpenScene(const std::filesystem::path& path) {
  if (scene_state_ != SceneState::kEdit) {
    OnScenePause();
  }

  Ref<Scene> new_scene = CreateRef<Scene>(GetState());
  SceneSerializer serializer(new_scene, GetState()->asset_library);
  if (serializer.Deserialize(path)) {
    editor_scene_ = new_scene;
    active_scene_ = editor_scene_;

    hierarchy_panel_->SetScene(active_scene_);

    editor_scene_path_ = path.string();
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

void EditorLayer::Exit() {
  GetState()->running = false;
}

void EditorLayer::OnSceneModify() {
  auto window = GetState()->window;
  std::string title = window->GetTitle();
  window->SetTitle(title + " *");

  modified_title_updated_ = true;
}

void EditorLayer::OnSceneSave() {
  SetSceneTitle();

  inspector_panel_->ResetModified();
  modified_title_updated_ = false;
}

void EditorLayer::SetSceneTitle() {
  GetState()->window->SetTitle(
      std::format("Eve Editor | {0}", Project::GetActive()->GetConfig().name));
}
