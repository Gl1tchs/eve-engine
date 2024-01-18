// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

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

int main(int argc, const char* argv[]) {
  eve::Ref<eve::State> state = eve::CreateRef<eve::State>();
  state->running = true;
  state->renderer = nullptr;

  eve::WindowCreateInfo info{};
  info.size = {800, 400};
  info.title = "Eve Project Manager";
  info.vsync = true;

  state->window = eve::CreateRef<eve::Window>(info);

  eve::SubscribeEvent<eve::WindowCloseEvent>(
      [&state](const eve::WindowCloseEvent& event) { state->running = false; });

  eve::Ref<eve::GraphicsContext> graphics_context =
      eve::GraphicsContext::Create();
  graphics_context->Init();

  eve::RenderCommand::Init();

  eve::Scope<eve::ImGuiLayer> imgui_layer =
      eve::CreateScope<eve::ImGuiLayer>(state);
  eve::Scope<UiLayer> ui_layer = eve::CreateScope<UiLayer>(state);

  eve::ImGuiSettings& settings = imgui_layer->GetSettings();
  settings.enable_docking = false;
  settings.enable_viewports = false;

  imgui_layer->OnCreate();
  ui_layer->OnCreate();

  eve::Timer timer;
  while (state->running) {

    float ds = timer.GetDeltaTime();

    imgui_layer->OnUpdate(ds);
    ui_layer->OnUpdate(ds);

    const auto& window_size = state->window->GetSize();

    eve::RenderCommand::Clear();
    eve::RenderCommand::SetClearColor(eve::kColorBlack);
    eve::RenderCommand::SetViewport(0, 0, window_size.x, window_size.y);

    imgui_layer->Begin();

    ui_layer->OnGUI(ds);

    imgui_layer->End();

    state->window->SwapBuffers();
  }

  ui_layer->OnDestroy();
  imgui_layer->OnDestroy();

  return 0;
}
