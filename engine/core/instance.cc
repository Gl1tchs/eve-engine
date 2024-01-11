#include "core/instance.h"

#include "core/event/event_handler.h"
#include "core/event/window_event.h"
#include "core/utils/timer.h"
#include "scripting/script_engine.h"

namespace eve {

Instance* Instance::instance_ = nullptr;

Instance::Instance(const InstanceSpecifications& specs) : specs_(specs) {
  ASSERT(!instance_, "Only one instance can exists.");
  instance_ = this;

  state_ = CreateRef<State>();
}

Instance::~Instance() {
  if (!ScriptEngine::IsInitialized()) {
    return;
  }

  ScriptEngine::Shutdown();
}

void Instance::Init() {
  // TODO get this from project config
  WindowProps props;
  props.title = specs_.name;
  props.size = {1680, 900};
  props.mode = WindowMode::kWindowed;
  props.vsync = true;

  state_->window = CreateRef<Window>(props);

  SubscribeEvent<WindowCloseEvent>(
      [this](const WindowCloseEvent& event) { state_->running = false; });

  state_->renderer = CreateRef<Renderer>();

  imgui_layer_ = new ImGuiLayer(state_);
  PushOverlay(imgui_layer_);
}

void Instance::StartEventLoop() {
  Timer timer;
  while (state_->running) {
    float ds = timer.GetDeltaTime();

    ProcessMainThreadQueue();

    for (Layer* layer : layers_) {
      layer->OnUpdate(ds);
    }

    imgui_layer_->Begin();
    {
      for (Layer* layer : layers_) {
        layer->OnGUI(ds);
      }
    }
    imgui_layer_->End();

    state_->window->SwapBuffers();
  }
}

void Instance::EnqueueMain(const std::function<void()>& function) {
  std::scoped_lock<std::mutex> lock(main_thread_queue_mutex_);

  main_thread_queue_.emplace_back(function);
}

void Instance::ProcessMainThreadQueue() {
  std::scoped_lock<std::mutex> lock(main_thread_queue_mutex_);
  for (auto& func : main_thread_queue_) {
    func();
  }

  main_thread_queue_.clear();
}

void Instance::PushLayer(Layer* layer) {
  layers_.PushLayer(layer);
}

void Instance::PushOverlay(Layer* overlay) {
  layers_.PushOverlay(overlay);
}

}  // namespace eve