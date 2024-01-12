// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_renderer_api.h"

#include <glad/glad.h>

namespace eve {

void OpenGLRendererAPI::Init() {
  glEnable(GL_BLEND);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_LINE_SMOOTH);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w,
                                    uint32_t h) {
  glViewport(x, y, w, h);
}

void OpenGLRendererAPI::SetClearColor(const Color& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

[[nodiscard]] static bool HasFlag(uint16_t flags, BufferBits flag) {
  return (flags & flag) != 0;
}

void OpenGLRendererAPI::Clear(uint16_t bits) {
  uint16_t flags = 0;

  if (HasFlag(bits, BufferBits_kColor)) {
    flags |= GL_COLOR_BUFFER_BIT;
  }

  if (HasFlag(bits, BufferBits_kDepth)) {
    flags |= GL_DEPTH_BUFFER_BIT;
  }

  glClear(flags);
}

void OpenGLRendererAPI::DrawArrays(const Ref<VertexArray>& vertex_array,
                                   uint32_t vertex_count) {
  vertex_array->Bind();
  glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array,
                                    uint32_t index_count) {
  vertex_array->Bind();
  uint32_t count =
      index_count ? index_count : vertex_array->GetIndexBuffer()->GetCount();
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertex_array,
                                  uint32_t vertex_count) {
  vertex_array->Bind();
  glDrawArrays(GL_LINES, 0, vertex_count);
}

void OpenGLRendererAPI::DrawArraysInstanced(
    const Ref<VertexArray>& vertex_array, uint32_t vertex_count,
    uint32_t instance_count) {
  vertex_array->Bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_count, instance_count);
}

void OpenGLRendererAPI::SetLineWidth(float width) {
  glLineWidth(width);
}

void OpenGLRendererAPI::SetPolygonMode(PolygonMode mode) {
  switch (mode) {
    case PolygonMode::kFill:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      return;
    case PolygonMode::kLine:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      return;
    case PolygonMode::kPoint:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      return;
    default:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      return;
  }
}

void OpenGLRendererAPI::SetDepthFunc(DepthFunc func) {
  int gl_func;
  switch (func) {
    case DepthFunc::kLess:
      gl_func = GL_LESS;
      break;
    case DepthFunc::kLEqual:
      gl_func = GL_LEQUAL;
      break;
    default:
      gl_func = GL_LESS;
      break;
  }

  glDepthFunc(gl_func);
}

void OpenGLRendererAPI::SetActiveTexture(uint8_t index) {
  glActiveTexture(GL_TEXTURE0 + index);
}

}  // namespace eve
