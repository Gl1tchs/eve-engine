// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "runtime_layer.h"

#include "core/instance.h"
#include "graphics/render_command.h"
#include "scene/scene_manager.h"

namespace eve {
RuntimeLayer::RuntimeLayer(Ref<State>& state) : Layer(state) {}

void RuntimeLayer::OnStart() {
  scene_renderer_ = CreateScope<SceneRenderer>(GetState());

  if (!SceneManager::GetActive()->OnRuntimeStart()) {
    LOG_RUNTIME_ERROR("Unable to start scene!");
    GetState()->running = false;
  }
}

void RuntimeLayer::OnDestroy() {}

void RuntimeLayer::OnUpdate(float ds) {
  const auto& window = GetState()->window;
  const glm::ivec2 viewport_size = window->GetSize();

  scene_renderer_->OnViewportResize(
      {(uint32_t)viewport_size.x, (uint32_t)viewport_size.y});

  RenderCommand::SetViewport(0, 0, (uint32_t)viewport_size.x,
                             (uint32_t)viewport_size.y);
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
  RenderCommand::Clear(BufferBits_kColor | BufferBits_kDepth);

  if (auto& scene = SceneManager::GetActive(); scene) {
    scene->OnUpdateRuntime(ds);
    scene_renderer_->RenderRuntime(ds);
  }
}
}  // namespace eve
