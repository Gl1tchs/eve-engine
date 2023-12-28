// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/primitives/mesh.h"

#include "graphics/render_command.h"
#include "graphics/renderer.h"

MeshPrimitive::MeshPrimitive()
    : Primitive<MeshVertex>(kMeshMaxVertexCount),
      index_offset(0),
      texture_slot_index_(0) {
  vertex_array_ = VertexArray::Create();

  // initialize vertex buffer
  vertex_buffer_ =
      VertexBuffer::Create(kMeshMaxVertexCount * sizeof(MeshVertex));
  vertex_buffer_->SetLayout({
      {ShaderDataType::kFloat4, "a_pos"},
      {ShaderDataType::kFloat3, "a_albedo"},
      {ShaderDataType::kFloat, "a_metallic"},
      {ShaderDataType::kFloat, "a_roughness"},
      {ShaderDataType::kFloat, "a_ao"},
      {ShaderDataType::kFloat3, "a_normal"},
      {ShaderDataType::kFloat2, "a_tex_coords"},
      {ShaderDataType::kFloat, "a_tex_index"},
      {ShaderDataType::kMat3, "a_normal_matrix"},
  });
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  // initialize index buffer
  index_buffer_ = IndexBuffer::Create(kMeshMaxIndexCount * sizeof(uint32_t));
  vertex_array_->SetIndexBuffer(index_buffer_);

  shader_ =
      Shader::Create("assets/shaders/pbr.vert", "assets/shaders/pbr.frag");

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

  uint32_t color = 0xffffff;
  white_texture_ = Texture::Create(metadata, &color);

  // Fill texture slots with default white texture
  std::fill(std::begin(texture_slots_), std::end(texture_slots_),
            white_texture_);
}

void MeshPrimitive::Render(RenderStats& stats) {
  const size_t index_count = indices_.size();
  if (index_count <= 0) {
    return;
  }

  index_buffer_->SetData(indices_.data(), index_count * sizeof(uint32_t));
  vertex_buffer_->SetData(GetBatches(), BatchCount() * sizeof(MeshVertex));

  // Bind textures
  for (uint32_t i = 0; i <= texture_slot_index_; i++) {
    texture_slots_[i]->Bind(i);
  }

  shader_->Bind();
  RenderCommand::DrawIndexed(vertex_array_, index_count);

  stats.draw_calls++;
}

bool MeshPrimitive::NeedsNewBatch(uint32_t vertex_size, uint32_t index_size) {
  return BatchCount() + vertex_size >= kMeshMaxVertexCount ||
         indices_.size() + index_size >= kMeshMaxIndexCount ||
         texture_slot_index_ >= kMeshMaxTextures;
}

void MeshPrimitive::AddIndex(uint32_t index) {
  if (!IsMutable()) {
    return;
  }

  indices_.push_back(index_offset + index);
}

void MeshPrimitive::Reset() {
  Primitive<MeshVertex>::Reset();

  indices_.clear();
  index_offset = 0;

  texture_slot_index_ = 1;
}

float MeshPrimitive::FindTexture(const Ref<Texture>& texture) {
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
