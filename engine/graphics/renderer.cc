// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/renderer.h"

#include <numeric>

#include "core/core_minimal.h"
#include "graphics/graphics.h"
#include "graphics/render_command.h"
#include "renderer.h"

Renderer::Renderer() {
  PROFILE_FUNCTION();

  GraphicsContext::Create()->Init();

  RenderCommand::Init();

  shader_ = Shader::Create(std::filesystem::path("shaders/default.vert.spirv"),
                           std::filesystem::path("shaders/default.frag.spirv"));

  shader_->Bind();

  int samplers[32];
  // fill the textures with empty values
  std::iota(std::begin(samplers), std::end(samplers), 0);
  shader_->SetUniform("u_textures", 32, samplers);

  vertex_array_ = VertexArray::Create();

  // initialize vertex buffer
  vertices_ = new Vertex[kMaxVertexCount];
  vertex_buffer_ = VertexBuffer::Create(kMaxVertexCount * sizeof(Vertex));

  BufferLayout layout = {{ShaderDataType::kFloat4, "a_pos"},
                         {ShaderDataType::kFloat4, "a_color"},
                         {ShaderDataType::kFloat2, "a_tex_coords"},
                         {ShaderDataType::kFloat, "a_tex_index"}};
  vertex_buffer_->SetLayout(layout);

  vertex_array_->AddVertexBuffer(vertex_buffer_);

  // initialize index buffer
  indices_ = new uint32_t[kMaxIndexCount];
  index_buffer_ = IndexBuffer::Create(kMaxIndexCount * sizeof(uint32_t));
  vertex_array_->SetIndexBuffer(index_buffer_);

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

  camera_uniform_buffer_ = UniformBuffer::Create(sizeof(CameraData), 0);
}

Renderer::~Renderer() {
  PROFILE_FUNCTION();

  delete[] vertices_;
  delete[] indices_;
}

void Renderer::BeginScene(const CameraData& camera_data) {
  PROFILE_FUNCTION();

  camera_uniform_buffer_->SetData(&camera_data, sizeof(CameraData));

  BeginBatch();
}

void Renderer::EndScene() {
  PROFILE_FUNCTION();

  Flush();
}

void Renderer::Draw(const RenderPacket& packet) {
  PROFILE_FUNCTION();

  if (NeedsNewBatch(packet.indices.size())) {
    NextBatch();
  }

  float texture_index = 0.0f;
  for (Vertex vertex : packet.vertices) {
    vertex.tex_index = texture_index;
    vertices_[vertex_count_++] = vertex;
  }

  for (const uint32_t& index : packet.indices) {
    indices_[index_count_++] = index_offset_ + index;
  }

  index_offset_ += packet.vertices.size();

  stats_.index_count += packet.indices.size();
  stats_.vertex_count += packet.vertices.size();
}

void Renderer::Draw(const RenderPacket& packet, const Ref<Texture>& texture) {
  PROFILE_FUNCTION();

  if (NeedsNewBatch(packet.indices.size()) ||
      texture_slot_index_ >= kMaxTextures) {
    NextBatch();
  }

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

  for (Vertex vertex : packet.vertices) {
    vertex.tex_index = texture_index;
    vertices_[vertex_count_++] = vertex;
  }

  for (const uint32_t& index : packet.indices) {
    indices_[index_count_++] = index_offset_ + index;
  }

  index_offset_ += packet.vertices.size();

  stats_.index_count += packet.indices.size();
  stats_.vertex_count += packet.vertices.size();
}

void Renderer::ResetStats() {
  memset(&stats_, 0, sizeof(RenderStats));
}

bool Renderer::NeedsNewBatch(uint32_t index_count) {
  return (index_count_ + index_count) >= kMaxIndexCount;
}

void Renderer::BeginBatch() {
  vertex_count_ = 0;

  index_count_ = 0;
  index_offset_ = 0;

  texture_slot_index_ = 1;
}

void Renderer::Flush() {
  index_buffer_->SetData(indices_, index_count_ * sizeof(uint32_t));
  vertex_buffer_->SetData(vertices_, vertex_count_ * sizeof(Vertex));

  RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
  RenderCommand::Clear(BufferBits_kColor | BufferBits_kDepth);

  if (!index_count_) {
    return;
  }

  for (uint32_t i = 0; i <= texture_slot_index_; i++) {
    texture_slots_[i]->Bind(i);
  }

  // TODO multiple shaders
  shader_->Bind();
  RenderCommand::DrawIndexed(vertex_array_, index_count_);

  stats_.draw_calls++;
}

void Renderer::NextBatch() {
  Flush();
  BeginBatch();
}
