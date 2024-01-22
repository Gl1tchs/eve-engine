// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/primitives/line.h"

#include "graphics/render_command.h"
#include "graphics/renderer.h"
#include "line.h"

namespace eve {

LinePrimitive::LinePrimitive() {
  vertex_array_ = VertexArray::Create();

  vertices_.Allocate(kMaxLineVertexCount);

  vertex_buffer_ = VertexBuffer::Create(vertices_.GetSize());
  vertex_buffer_->SetLayout({{ShaderDataType::kFloat3, "a_position"},
                             {ShaderDataType::kFloat4, "a_color"}});
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  shader_ =
      Shader::Create("assets/shaders/line.vert", "assets/shaders/line.frag");
}

void LinePrimitive::Render(RenderStats& stats) {
  if (vertices_.GetCount() <= 0) {
    return;
  }

  vertex_buffer_->SetData(vertices_.GetData(), vertices_.GetSize());

  shader_->Bind();
  RenderCommand::SetLineWidth(line_width);
  RenderCommand::DrawLines(vertex_array_, vertices_.GetCount());

  stats.draw_calls++;
}

void LinePrimitive::Reset() {
  vertices_.ResetIndex();
}

void LinePrimitive::AddInstance(const glm::vec3& p0, const glm::vec3& p1,
                                const Color& color) {
  LineVertex v0;
  v0.position = p0;
  v0.color = color;
  vertices_.Add(v0);

  LineVertex v1;
  v1.position = p1;
  v1.color = color;
  vertices_.Add(v1);
}

bool LinePrimitive::NeedsNewBatch() {
  return vertices_.GetCount() + 2 >= kMaxLineVertexCount;
}

}  // namespace eve