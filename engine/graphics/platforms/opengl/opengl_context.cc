// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "graphics/graphics_context.h"

static void GLAPIENTRY OpenGLMessageCallback(uint32_t source, uint32_t type,
                                             uint32_t id, uint32_t severity,
                                             int32_t length,
                                             const char* message,
                                             const void* user_param);

void OpenGLContext::Init() {
  // TODO if you add different window classes update this
  int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  ASSERT(status, "Failed to initialize Glad!");

#ifdef EVE_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif

  LOG_TRACE("OpenGL Info:");
  LOG_TRACE("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
  LOG_TRACE("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
  LOG_TRACE("  Version: {0}", (const char*)glGetString(GL_VERSION));

  ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
         "Requires at least OpenGL version of 4.5!");
}

DeviceInformation OpenGLContext::GetDeviceInfo() const {
  DeviceInformation info;
  info.vendor = (const char*)glGetString(GL_VENDOR);
  info.renderer = (const char*)glGetString(GL_RENDERER);
  return info;
}

void GLAPIENTRY OpenGLMessageCallback(uint32_t source, uint32_t type,
                                      uint32_t id, uint32_t severity, int32_t,
                                      const char* message, const void*) {
  // Convert GLenum parameters to strings
  std::string source_string;
  switch (source) {
    case GL_DEBUG_SOURCE_API:
      source_string = "API";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      source_string = "Window System";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      source_string = "Shader Compiler";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      source_string = "Third Party";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      source_string = "Application";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      source_string = "Other";
      break;
    default:
      source_string = "Unknown";
      break;
  }

  std::string type_string;
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      type_string = "Error";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      type_string = "Deprecated Behavior";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      type_string = "Undefined Behavior";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      type_string = "Portability";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      type_string = "Performance";
      break;
    case GL_DEBUG_TYPE_MARKER:
      type_string = "Marker";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      type_string = "Push Group";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      type_string = "Pop Group";
      break;
    case GL_DEBUG_TYPE_OTHER:
      type_string = "Other";
      break;
    default:
      type_string = "Unknown";
      break;
  }

  std::string severity_string;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      severity_string = "High";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      severity_string = "Medium";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      severity_string = "Low";
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      severity_string = "Notification";
      break;
    default:
      severity_string = "Unknown";
      break;
  }

  LOG_TRACE("OpenGL Debug Message:");
  LOG_TRACE("Source: {0}", source_string);
  LOG_TRACE("Type: {0}", type_string);
  LOG_TRACE("ID: {0}", id);
  LOG_TRACE("Severity: {0}", severity_string);
  LOG_TRACE("Message: {0}", message);
}