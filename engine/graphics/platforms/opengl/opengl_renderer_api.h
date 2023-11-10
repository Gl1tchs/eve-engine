// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "graphics/renderer_api.h"

class OpenGLRendererAPI final : public RendererAPI {
 public:
  void Init() override;

  void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
  void SetClearColor(const glm::vec4& color) override;
  void Clear(uint16_t bits = BufferBits_kColor) override;

  void DrawIndexed(const Ref<VertexArray>& vertex_array,
                   uint32_t index_Count = 0) override;
  void DrawLines(const Ref<VertexArray>& vertex_array,
                 uint32_t vertex_count) override;

  void SetLineWidth(float width) override;
};
