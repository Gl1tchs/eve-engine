#pragma once

#include "core/core_minimal.h"
#include "core/event/event_handler.h"
#include "core/event/window_event.h"
#include "core/layer.h"
#include "core/utils/timer.h"
#include "core/window.h"
#include "graphics/graphics_context.h"
#include "graphics/render_command.h"
#include "ui/imgui_layer.h"

#include "ui_layer.h"

using namespace eve;

int main(int argc, const char* argv[]) {
  Ref<State> state = CreateRef<State>();
  state->running = true;
  state->renderer = nullptr;

  WindowCreateInfo info{};
  info.size = {800, 400};
  info.title = "Eve Project Manager";
  info.vsync = true;

  state->window = CreateRef<Window>(info);

  SubscribeEvent<WindowCloseEvent>(
      [&state](const WindowCloseEvent& event) { state->running = false; });

  Ref<GraphicsContext> graphics_context = GraphicsContext::Create();
  graphics_context->Init();

  RenderCommand::Init();

  ImGuiLayer* imgui_layer = new ImGuiLayer(state);
  UiLayer* ui_layer = new UiLayer(state);

  ImGuiSettings& settings = imgui_layer->GetSettings();
  settings.enable_docking = false;
  settings.enable_viewports = false;

  imgui_layer->OnStart();
  ui_layer->OnStart();

  Timer timer;
  while (state->running) {

    float ds = timer.GetDeltaTime();

    imgui_layer->OnUpdate(ds);
    ui_layer->OnUpdate(ds);

    const auto& window_size = state->window->GetSize();

    RenderCommand::Clear();
    RenderCommand::SetClearColor(kColorBlack);
    RenderCommand::SetViewport(0, 0, window_size.x, window_size.y);

    imgui_layer->Begin();

    ui_layer->OnGUI(ds);

    imgui_layer->End();

    state->window->SwapBuffers();
  }

  ui_layer->OnDestroy();
  imgui_layer->OnDestroy();

  return 0;
}
