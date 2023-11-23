// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_renderer_api.h"

#include <glad/glad.h>

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

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.x, color.y, color.z, color.w);
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
