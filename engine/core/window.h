// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_CORE_WINDOW_H_
#define EVE_CORE_WINDOW_H_

#include <string>

#include <glm/glm.hpp>

namespace {
struct GLFWwindow;
}  // namespace

enum class WindowMode {
  kWindowed,
  kFullscreen,
  kWindowedFullscreen,
};

struct WindowProps {
  std::string title;
  glm::ivec2 size;
  WindowMode mode;
  bool vsync;
  // TODO icon
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

  [[nodiscard]] void* GetNativeWindow();

 private:
  void InitEvents();

  GLFWwindow* window_;
  std::string title_;
  glm::ivec2 size_;
  bool vsync_;
  WindowMode mode_;
};

#endif  // EVE_CORE_WINDOW_H_
