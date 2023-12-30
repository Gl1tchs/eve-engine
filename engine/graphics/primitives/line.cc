// Copyright (c) 2023   Berke Umut Biricik All Rights Reserved

#include "graphics/primitives/line.h"

#include "graphics/render_command.h"
#include "graphics/renderer.h"

namespace eve {
LinePrimitive::LinePrimitive() : Primitive<LineVertex>(kMaxLines) {
  vertex_array_ = VertexArray::Create();

  vertex_buffer_ = VertexBuffer::Create(kMaxLines * sizeof(LineVertex));
  vertex_buffer_->SetLayout({{ShaderDataType::kFloat3, "a_pos"},
                             {ShaderDataType::kFloat4, "a_color"}});
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  shader_ =
      Shader::Create("assets/shaders/line.vert", "assets/shaders/line.frag");
}

void LinePrimitive::Render(RenderStats& stats) {
  const size_t& batch_count = BatchCount();

  if (batch_count <= 0) {
    return;
  }

  vertex_buffer_->SetData(GetBatches(), batch_count * sizeof(LineVertex));

  shader_->Bind();
  RenderCommand::SetLineWidth(line_width);
  RenderCommand::DrawLines(vertex_array_, batch_count);

  stats.draw_calls++;
}

bool LinePrimitive::NeedsNewBatch() {
  return (BatchCount() + 2) >= kMaxLines;
}
}  // namespace eve