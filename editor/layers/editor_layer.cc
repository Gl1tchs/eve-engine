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

  active_scene_ = CreateRef<Scene>(state_);

  frame_buffer_ = FrameBuffer::Create({300, 300});

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

  active_scene_->OnViewportResize(
      {(uint32_t)viewport_size_.x, (uint32_t)viewport_size_.y});

  // Resize
  if (glm::vec2 size = frame_buffer_->GetSize();
      viewport_size_.x > 0.0f &&
      viewport_size_.y > 0.0f &&  // zero sized framebuffer is invalid
      (size.x != viewport_size_.x || size.y != viewport_size_.y)) {
    frame_buffer_->SetSize(
        {(uint32_t)viewport_size_.x, (uint32_t)viewport_size_.y});
    editor_camera_.SetAspectRatio(viewport_size_.x / viewport_size_.y);
  }

  Ref<Renderer>& renderer = state_->renderer;

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

  // Note: Switch this to true to enable dockspace
  static bool dockspace_open = true;
  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
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
  }

  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
  ImGui::PopStyleVar();

  if (opt_fullscreen)
    ImGui::PopStyleVar(2);

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
        state_->running = false;
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Render Stats")) {
        show_stats_ = !show_stats_;
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  if (show_stats_) {
    ImGui::Begin("Stats", &show_stats_);

    auto stats = state_->renderer->GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Vertices: %d", stats.vertex_count);
    ImGui::Text("Indices: %d", stats.index_count);

    ImGui::End();
  }

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin("Viewport");
  auto viewport_min_region = ImGui::GetWindowContentRegionMin();
  auto viewport_max_region = ImGui::GetWindowContentRegionMax();
  auto viewport_offset = ImGui::GetWindowPos();
  viewport_bounds_[0] = {viewport_min_region.x + viewport_offset.x,
                         viewport_min_region.y + viewport_offset.y};
  viewport_bounds_[1] = {viewport_max_region.x + viewport_offset.x,
                         viewport_max_region.y + viewport_offset.y};

  viewport_focused_ = ImGui::IsWindowFocused();
  viewport_hovered_ = ImGui::IsWindowHovered();

  ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
  viewport_size_ = {viewport_panel_size.x, viewport_panel_size.y};

  uint64_t textureID = frame_buffer_->GetTexture()->GetTextureID();
  ImGui::Image(reinterpret_cast<void*>(textureID),
               ImVec2{viewport_size_.x, viewport_size_.y}, ImVec2{0, 1},
               ImVec2{1, 0});

  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();
}
