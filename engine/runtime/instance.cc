#include "runtime/instance.h"

#include "core/event/event_handler.h"
#include "core/event/events.h"
#include "core/utils/timer.h"

Instance::Instance() {
  PROFILE_FUNCTION();

  // TODO get this from project config
  WindowProps props;
  props.title = "Eve Application";
  props.size = {800, 600};
  props.mode = WindowMode::kWindowed;
  props.vsync = true;

  window_ = CreateRef<Window>(props);

  SubscribeEvent<WindowCloseEvent>(
      [this](const WindowCloseEvent& event) { is_running_ = false; });
}

Instance::~Instance() {
  PROFILE_FUNCTION();
}

void Instance::StartEventLoop() {
  PROFILE_FUNCTION();

  Timer timer;
  while (is_running_) {
    PROFILE_SCOPE("EventLoop");

    timer.Tick();

    // TODO behave

    window_->SwapBuffers();
  }
}
