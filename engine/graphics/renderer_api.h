// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/utils/memory.h"
#include "graphics/vertex_array.h"

#define BIT(x) 1 << x

enum class GraphicsAPI { kNone = 0, kOpenGL, kVulkan };

enum BufferBits : uint16_t {
  BufferBits_kDepth = BIT(0),
  BufferBits_kColor = BIT(1),
};

class RendererAPI {
 public:
  virtual ~RendererAPI() = default;

  virtual void Init() = 0;

  virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;
  virtual void Clear(uint16_t bits = BufferBits_kColor) = 0;

  virtual void DrawIndexed(const Ref<VertexArray>& vertex_array,
                           uint32_t index_count = 0) = 0;
  virtual void DrawLines(const Ref<VertexArray>& vertex_array,
                         uint32_t vertex_count) = 0;

  virtual void DrawArraysInstanced(const Ref<VertexArray>& vertex_array,
                           uint32_t vertex_count, uint32_t instance_count) = 0;

  virtual void SetLineWidth(float width) = 0;

  [[nodiscard]] static Scope<RendererAPI> Create();
};
