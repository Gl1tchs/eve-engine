// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/primitives/quad.h"

#include "asset/asset_registry.h"
#include "core/file_system.h"
#include "graphics/render_command.h"
#include "graphics/renderer.h"
#include "quad.h"

namespace eve {

QuadPrimitive::QuadPrimitive() {
  vertex_array_ = VertexArray::Create();

  // initialize vertex buffer
  vertices_.Allocate(kQuadMaxVertexCount);

  vertex_buffer_ = VertexBuffer::Create(vertices_.GetSize());
  vertex_buffer_->SetLayout({
      {ShaderDataType::kFloat4, "a_position"},
      {ShaderDataType::kFloat2, "a_tex_coords"},
      {ShaderDataType::kFloat4, "a_color"},
      {ShaderDataType::kFloat, "a_tex_index"},
      {ShaderDataType::kFloat2, "a_tex_tiling"},
  });
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  // initialize index buffer
  uint32_t* indices = new uint32_t[kQuadMaxIndexCount];

  uint32_t offset = 0;
  for (uint32_t i = 0; i < kQuadMaxIndexCount; i += kQuadIndexCount) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;

    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;

    offset += kQuadVertexCount;
  }

  Ref<IndexBuffer> index_buffer =
      IndexBuffer::Create(indices, kQuadMaxIndexCount);
  vertex_array_->SetIndexBuffer(index_buffer);

  delete[] indices;

  shader_ = Shader::Create("assets/shaders/sprite.vert",
                           "assets/shaders/sprite.frag");

  // fill the textures with empty values (which is default white texture)
  {
    shader_->Bind();
    int samplers[32];
    std::iota(std::begin(samplers), std::end(samplers), 0);
    shader_->SetUniform("u_textures", 32, samplers);
  }

  // Create default 1x1 white texture
  TextureMetadata metadata;
  metadata.size = {1, 1};
  metadata.format = TextureFormat::kRGBA;
  metadata.min_filter = TextureFilteringMode::kLinear;
  metadata.mag_filter = TextureFilteringMode::kLinear;
  metadata.wrap_s = TextureWrappingMode::kClampToEdge;
  metadata.wrap_t = TextureWrappingMode::kClampToEdge;
  metadata.generate_mipmaps = false;

  uint32_t color = 0xffffffff;
  white_texture_ = Texture::Create(metadata, &color);

  // Fill texture slots with default white texture
  std::fill(std::begin(texture_slots_), std::end(texture_slots_),
            white_texture_);
}

QuadPrimitive::~QuadPrimitive() {}

void QuadPrimitive::Render(RenderStats& stats) {
  if (index_count_ <= 0) {
    return;
  }

  vertex_buffer_->SetData(vertices_.GetData(), vertices_.GetSize());

  // Bind textures
  for (uint32_t i = 0; i <= texture_slot_index_; i++) {
    texture_slots_[i]->Bind(i);
  }

  shader_->Bind();
  RenderCommand::DrawIndexed(vertex_array_, index_count_);

  stats.draw_calls++;
}

void QuadPrimitive::Reset() {
  vertices_.ResetIndex();
  index_count_ = 0;
  texture_slot_index_ = 1;
}

void QuadPrimitive::AddInstance(const Transform& transform, const Color& color,
                                const Ref<Texture>& texture,
                                const glm::vec2& tiling) {
  const float tex_index = texture ? FindTexture(texture) : 0.0f;
  const glm::mat4 transform_matrix = transform.GetTransformMatrix();

  for (size_t i = 0; i < kQuadVertexCount; i++) {
    QuadVertex vertex;

    vertex.position = transform_matrix * kQuadVertexPositions[i];
    vertex.tex_coords = kQuadVertexTexCoords[i];
    vertex.color = color;
    vertex.tex_index = tex_index;
    vertex.tex_tiling = tiling;

    vertices_.Add(vertex);
  }

  index_count_ += kQuadIndexCount;
}

bool QuadPrimitive::NeedsNewBatch() {
  return vertices_.GetCount() + kQuadVertexCount >= kQuadMaxVertexCount ||
         texture_slot_index_ >= kQuadMaxTextures;
}

float QuadPrimitive::FindTexture(const Ref<Texture>& texture) {
  float texture_index = 0.0f;
  for (uint32_t i = 1; i < texture_slot_index_; i++) {
    if (texture_slots_[i] == texture) {
      texture_index = (float)i;
    }
  }

  if (texture_index == 0.0f) {
    texture_index = (float)texture_slot_index_;
    texture_slots_[texture_slot_index_++] = texture;
  }

  return texture_index;
}

}  // namespace eve