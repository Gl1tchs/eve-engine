// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/renderer_api.h"

namespace eve {
class OpenGLRendererAPI final : public RendererAPI {
 public:
  void Init() override;

  void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
  void SetClearColor(const glm::vec4& color) override;
  void Clear(uint16_t bits = BufferBits_kColor) override;

  void DrawArrays(const Ref<VertexArray>& vertex_array,
                  uint32_t vertex_count) override;
  void DrawIndexed(const Ref<VertexArray>& vertex_array,
                   uint32_t index_Count = 0) override;

  void DrawLines(const Ref<VertexArray>& vertex_array,
                 uint32_t vertex_count) override;

  void DrawArraysInstanced(const Ref<VertexArray>& vertex_array,
                           uint32_t vertex_count,
                           uint32_t instance_count) override;

  void SetLineWidth(float width) override;

  void SetDepthFunc(DepthFunc func = DepthFunc::kLess) override;

  void SetActiveTexture(uint8_t index = 0) override;
};
}  // namespace eve
