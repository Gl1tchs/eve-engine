// Copyright (c) 2023   Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/buffer.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"

namespace eve {

struct RenderStats;

constexpr size_t kMaxLines = 2000;

constexpr size_t kLineVertexCount = 2;
constexpr size_t kMaxLineVertexCount = kMaxLines * kLineVertexCount;

struct LineVertex {
  glm::vec3 position;
  Color color;
};

class LinePrimitive {
 public:
  LinePrimitive();
  virtual ~LinePrimitive() = default;

  float line_width = 0.5f;

  void Render(RenderStats& stats);

  void Reset();

  void AddInstance(const glm::vec3& p0, const glm::vec3& p1,
                   const Color& color);

  [[nodiscard]] bool NeedsNewBatch();

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<Shader> shader_;

  BufferArray<LineVertex> vertices_;
};

}  // namespace eve