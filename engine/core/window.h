// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

struct GLFWwindow;

namespace eve {

enum class CursorState { kNormal, kHidden };

enum class WindowMode {
  kWindowed,
  kFullscreen,
  kWindowedFullscreen,
};

struct WindowProps {
  std::string title =
      "Eve Project";  // TODO add the rendering api to the title ex: "Eve Project | OpenGL"
  glm::ivec2 size = {800, 600};
  WindowMode mode = WindowMode::kWindowed;
  bool vsync = true;
  // TODO get icon by res://icon.ico if not use default
};

class Window final {
 public:
  Window(WindowProps props);
  ~Window();

  [[nodiscard]] bool IsOpen();

  void SwapBuffers();

  [[nodiscard]] bool GetVsync();
  void SetVsync(bool value);

  [[nodiscard]] WindowMode GetMode();
  void SetMode(const WindowMode& mode);

  [[nodiscard]] glm::ivec2 GetSize();
  void SetSize(const glm::ivec2& size);

  [[nodiscard]] bool IsMinimized();

  [[nodiscard]] std::string GetTitle();
  void SetTitle(const std::string& value);

  void SetCursorState(CursorState state);

  [[nodiscard]] GLFWwindow* GetNativeWindow();

 private:
  void InitEvents();

  GLFWwindow* window_;
  std::string title_;
  glm::ivec2 size_;
  bool vsync_;
  WindowMode mode_;
};

}  // namespace eve