#include "runtime/instance.h"

#include "core/event/event_handler.h"
#include "core/event/events.h"
#include "core/utils/timer.h"

#include "graphics/orthographic_camera.h"
#include "graphics/render_command.h"

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

  renderer_ = CreateRef<Renderer>();
}

Instance::~Instance() {
  PROFILE_FUNCTION();
}

void Instance::StartEventLoop() {
  PROFILE_FUNCTION();

  Transform camera_transform{};
  OrthographicCamera camera(800 / 600);

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

  Timer timer;
  while (is_running_) {
    PROFILE_SCOPE("EventLoop");

    timer.Tick();

    auto window_size = window_->GetSize();
    camera.SetAspectRatio(window_size.x / window_size.y);

    RenderCommand::SetViewport(0, 0, window_size.x, window_size.y);

    renderer_->BeginScene(
        {camera.GetViewMatrix(camera_transform), camera.GetProjectionMatrix()});

    renderer_->Draw(packet, texture);

    renderer_->EndScene();

    window_->SwapBuffers();
  }
}
