#include "core/instance.h"

#include "core/event/event_handler.h"
#include "core/event/events.h"
#include "core/utils/timer.h"
#include "instance.h"

Instance* Instance::instance_ = nullptr;

Instance::Instance(const InstanceSpecifications& specs) : specs_(specs) {
  PROFILE_FUNCTION();

  ENGINE_ASSERT(!instance_, "Only one instance can exists.");
  instance_ = this;

  state_ = CreateRef<State>();

  // TODO get this from project config
  WindowProps props;
  props.title = specs_.name;
  props.size = {800, 600};
  props.mode = WindowMode::kWindowed;
  props.vsync = true;

  state_->window = CreateRef<Window>(props);

  SubscribeEvent<WindowCloseEvent>(
      [this](const WindowCloseEvent& event) { state_->running = false; });

  state_->renderer = CreateRef<Renderer>();

  imgui_layer_ = new ImGuiLayer(state_);
  PushOverlay(imgui_layer_);
}

Instance::~Instance() {
  PROFILE_FUNCTION();
}

void Instance::StartEventLoop() {
  PROFILE_FUNCTION();

  Timer timer;
  while (state_->running) {
    PROFILE_SCOPE("EventLoop");

    float ds = timer.Tick();

    PROFILE_SCOPE("LayerStack OnUpdate");
    for (Layer* layer : layers_) {
      layer->OnUpdate(ds);
    }

    imgui_layer_->Begin();
    {
      PROFILE_SCOPE("LayerStack OnGUI");

      for (Layer* layer : layers_) {
        layer->OnGUI(ds);
      }
    }
    imgui_layer_->End();

    state_->window->SwapBuffers();
  }
}

void Instance::PushLayer(Layer* layer) {
  layers_.PushLayer(layer);
}

void Instance::PushOverlay(Layer* overlay) {
  layers_.PushOverlay(overlay);
}

void Instance::PopLayer(Layer* layer) {
  layers_.PopLayer(layer);
}

void Instance::PopOverlay(Layer* overlay) {
  layers_.PopOverlay(overlay);
}
