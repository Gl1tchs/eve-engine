// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/color.h"
#include "graphics/vertex_array.h"

namespace eve {
enum class GraphicsAPI { kNone = 0, kOpenGL, kVulkan };

enum BufferBits : uint16_t {
  BufferBits_kDepth = BIT(0),
  BufferBits_kColor = BIT(1),
};

enum class DepthFunc { kLess, kLEqual };

enum class PolygonMode { kFill, kLine, kPoint };

class RendererAPI {
 public:
  virtual ~RendererAPI() = default;

  virtual void Init() = 0;

  virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
  virtual void SetClearColor(const Color& color) = 0;
  virtual void Clear(uint16_t bits = BufferBits_kColor) = 0;

  virtual void DrawArrays(const Ref<VertexArray>& vertex_array,
                          uint32_t vertex_count) = 0;
  virtual void DrawIndexed(const Ref<VertexArray>& vertex_array,
                           uint32_t index_count = 0) = 0;

  virtual void DrawLines(const Ref<VertexArray>& vertex_array,
                         uint32_t vertex_count) = 0;

  virtual void DrawArraysInstanced(const Ref<VertexArray>& vertex_array,
                                   uint32_t vertex_count,
                                   uint32_t instance_count) = 0;

  virtual void SetLineWidth(float width) = 0;

  virtual void SetPolygonMode(PolygonMode mode = PolygonMode::kFill) = 0;

  virtual void SetDepthFunc(DepthFunc func = DepthFunc::kLess) = 0;

  virtual void SetActiveTexture(uint8_t index = 0) = 0;

  [[nodiscard]] static Scope<RendererAPI> Create();
};
}  // namespace eve