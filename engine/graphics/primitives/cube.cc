// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/primitives/cube.h"

#include "asset/asset_registry.h"
#include "core/file_system.h"
#include "graphics/primitives/quad.h"
#include "graphics/render_command.h"
#include "graphics/renderer.h"

namespace eve {

CubePrimitive::CubePrimitive()
    : Primitive<CubeVertex>(kCubeMaxVertexCount), index_count_(0) {
  vertex_array_ = VertexArray::Create();

  // initialize vertex buffer
  vertex_buffer_ =
      VertexBuffer::Create(kCubeMaxVertexCount * sizeof(CubeVertex));
  vertex_buffer_->SetLayout({
      {ShaderDataType::kFloat4, "a_position"},
      {ShaderDataType::kFloat2, "a_tex_coords"},
      {ShaderDataType::kFloat4, "a_color"},
  });
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  // initialize index buffer
  uint32_t* indices = new uint32_t[kCubeMaxIndexCount];

  uint32_t offset = 0;

  // a cube is basicly 6 quads so this does not need to change
  for (uint32_t i = 0; i < kCubeMaxIndexCount; i += kQuadIndexCount) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;

    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;

    offset += kQuadVertexCount;
  }

  Ref<IndexBuffer> index_buffer =
      IndexBuffer::Create(indices, kCubeMaxIndexCount);
  vertex_array_->SetIndexBuffer(index_buffer);

  delete[] indices;

  shader_ =
      Shader::Create("assets/shaders/cube.vert", "assets/shaders/cube.frag");
}

void CubePrimitive::Render(RenderStats& stats) {
  if (index_count_ <= 0) {
    return;
  }

  vertex_buffer_->SetData(GetBatches(), BatchCount() * sizeof(CubeVertex));

  shader_->Bind();
  RenderCommand::DrawIndexed(vertex_array_, index_count_);

  stats.draw_calls++;
}

bool CubePrimitive::NeedsNewBatch() {
  return BatchCount() + kCubeVertexCount >= kCubeMaxVertexCount;
}

void CubePrimitive::OnReset() {
  index_count_ = 0;
}

}  // namespace eve