// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/renderer_api.h"

namespace eve {

class RenderCommand final {
 public:
  static void Init();

  static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
  static void SetClearColor(const Color& color);
  static void Clear(uint16_t bits = BufferBits_kColor);

  static void DrawArrays(const Ref<VertexArray>& vertex_array,
                         uint32_t vertex_count);
  static void DrawIndexed(const Ref<VertexArray>& vertex_array,
                          uint32_t index_count);

  static void DrawLines(const Ref<VertexArray>& vertex_array,
                        uint32_t vertex_count);

  static void DrawArraysInstanced(const Ref<VertexArray>& vertex_array,
                                  uint32_t vertex_count,
                                  uint32_t instance_count);

  static void SetLineWidth(float width);

  static void SetPolygonMode(PolygonMode mode = PolygonMode::kFill);

  static void SetDepthFunc(DepthFunc func = DepthFunc::kLess);

  static void SetActiveTexture(uint8_t index = 0);

 private:
  static Scope<RendererAPI> renderer_api_;
};

}  // namespace eve