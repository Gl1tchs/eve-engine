// Copyright (c) 2023   Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/primitives/primitive.h"

#include "graphics/shader.h"
#include "graphics/vertex_array.h"

constexpr size_t kMaxLines = 2000;

struct LineVertex {
  glm::vec3 position;
  glm::vec4 color;
};

template <>
struct RenderData<LineVertex> {
  std::vector<LineVertex> vertices;
};

class LinePrimitive : public Primitive<LineVertex> {
 public:
  LinePrimitive();
  virtual ~LinePrimitive() = default;

  float line_width = 0.5f;

  void Render(RenderStats& stats) override;

  [[nodiscard]] bool NeedsNewBatch() override;

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<Shader> shader_;
};