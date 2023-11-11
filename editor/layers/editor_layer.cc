// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "layers/editor_layer.h"

#include <imgui.h>

#include "core/debug/instrumentor.h"
#include "graphics/render_command.h"
#include "scene/entity.h"

EditorLayer::EditorLayer(Ref<State>& state) : Layer(state) {
  PROFILE_FUNCTION();

  editor_logger_ = LoggerManager::AddLogger("EDITOR");
}

EditorLayer::~EditorLayer() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnStart() {
  PROFILE_FUNCTION();

  active_scene_ = CreateRef<Scene>(GetState());

  frame_buffer_ = FrameBuffer::Create({300, 300});

  viewport_ = CreateScope<ViewportPanel>(frame_buffer_);

  render_stats_ = CreateScope<RenderStatsPanel>(GetState()->renderer);

  Entity entity = active_scene_->CreateEntity("My Entity");

  RenderPacket packet;
  packet.vertices = {
      {{-0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
      {{-0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
      {{0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
      {{0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  };
  packet.indices = {0, 1, 2, 2, 3, 0};

  // TODO add proper asset system and use res:// thing.
  Ref<Texture> texture = Texture::Create("assets/textures/checkerboard.png");

  DrawableComponent drawable;
  drawable.packet = packet;
  drawable.texture = texture;
  entity.AddComponent<DrawableComponent>(drawable);
}

void EditorLayer::OnDestroy() {
  PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(float ds) {
  PROFILE_FUNCTION();

  auto viewport_size = viewport_->GetSize();

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

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        GetState()->running = false;
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Render Stats")) {
        render_stats_->SetActive(true);
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  render_stats_->Render();

  viewport_->Render();

  ImGui::End();
}
