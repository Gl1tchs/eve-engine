// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/window.h"

#include <GLFW/glfw3.h>

#include "core/event/event_handler.h"
#include "core/event/input.h"
#include "core/event/key_event.h"
#include "core/event/mouse_event.h"
#include "core/event/window_event.h"

namespace eve {

void GlfwErrorCallback(int error, const char* description);

Window::Window(WindowProps props) {

  title_ = props.title;
  size_ = props.size;
  mode_ = props.mode;

  ASSERT(glfwInit(), "Could not initialize GLFW!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
  glfwSetErrorCallback(GlfwErrorCallback);
#endif

  GLFWmonitor* monitor = nullptr;
  if (mode_ != WindowMode::kWindowed) {
    monitor = glfwGetPrimaryMonitor();
  }

  if (mode_ == WindowMode::kWindowedFullscreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    size_ = {mode->width, mode->height};
  }

  window_ =
      glfwCreateWindow(size_.x, size_.y, title_.c_str(), monitor, nullptr);
  ASSERT(window_, "GLFW window could not created!")

  glfwMakeContextCurrent(window_);

  SetVsync(props.vsync);

  InitEvents();

  glm::dvec2 mouse_pos{};
  glfwGetCursorPos(window_, &mouse_pos.x, &mouse_pos.y);

  Input::Init(mouse_pos);
}

Window::~Window() {

  glfwDestroyWindow(window_);
  glfwTerminate();
}

bool Window::IsOpen() {

  return !glfwWindowShouldClose(window_);
}

void Window::SwapBuffers() {

  glfwPollEvents();
  glfwSwapBuffers(window_);
}

bool Window::GetVsync() {
  return vsync_;
}

void Window::SetVsync(bool value) {

  glfwSwapInterval(value);
  vsync_ = value;
}

WindowMode Window::GetMode() {
  return mode_;
}

void Window::SetMode(const WindowMode& mode) {

  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
  }

  GLFWmonitor* monitor = nullptr;
  if (mode != WindowMode::kWindowed) {
    monitor = glfwGetPrimaryMonitor();
  }

  if (mode == WindowMode::kWindowedFullscreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    size_ = {mode->width, mode->height};
  }

  window_ =
      glfwCreateWindow(size_.x, size_.y, title_.c_str(), monitor, nullptr);
  ASSERT(window_, "GLFW window could not created!")

  mode_ = mode;
}

glm::ivec2 Window::GetSize() {
  int w, h;
  glfwGetWindowSize(window_, &w, &h);
  return glm::ivec2{w, h};
}

void Window::SetSize(const glm::ivec2& size) {
  glfwSetWindowSize(window_, size.x, size.y);
}

bool Window::IsMinimized() {
  return glfwGetWindowAttrib(window_, GLFW_ICONIFIED) == GLFW_TRUE;
}

std::string Window::GetTitle() {
  return title_;
}

void Window::SetTitle(const std::string& value) {
  glfwSetWindowTitle(window_, value.c_str());
  title_ = value;
}

void Window::SetCursorState(CursorState state) {
  switch (state) {
    case CursorState::kNormal:
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;
    case CursorState::kHidden:
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      break;
    default:
      break;
  }
}

GLFWwindow* Window::GetNativeWindow() {
  return window_;
}

void Window::InitEvents() {

  glfwSetWindowSizeCallback(window_,
                            [](GLFWwindow* window, int width, int height) {
                              WindowResizeEvent event(width, height);
                              NotifyEvent(event);
                            });

  glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
    WindowCloseEvent event;
    NotifyEvent(event);
  });

  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    switch (action) {
      case GLFW_PRESS: {
        KeyPressEvent event(key, false);
        NotifyEvent(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleaseEvent event(key);
        NotifyEvent(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressEvent event(key, true);
        NotifyEvent(event);
        break;
      }
      default: {
        break;
      }
    }
  });

  glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int keycode) {
    KeyTypeEvent event(keycode);
    NotifyEvent(event);
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressEvent event(button);
            NotifyEvent(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleaseEvent event(button);
            NotifyEvent(event);
            break;
          }
          default: {
            break;
          }
        }
      });

  glfwSetCursorPosCallback(window_, [](GLFWwindow* window, const double x_pos,
                                       const double y_pos) {
    MouseMoveEvent event(static_cast<float>(x_pos), static_cast<float>(y_pos));
    NotifyEvent(event);
  });

  glfwSetScrollCallback(window_, [](GLFWwindow* window, const double x_offset,
                                    const double y_offset) {
    MouseScrollEvent event(static_cast<float>(x_offset),
                           static_cast<float>(y_offset));
    NotifyEvent(event);
  });
}

void GlfwErrorCallback(int error, const char* description) {
  LOG_ENGINE_ERROR("GLFW Error: (no:\"{0}\") {1}", error, description);
}

}  // namespace eve