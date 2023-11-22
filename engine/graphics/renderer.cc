// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/renderer.h"

#include <numeric>

#include "core/core_minimal.h"
#include "graphics/graphics.h"
#include "graphics/render_command.h"

Renderer::Renderer() {
  PROFILE_FUNCTION();

  GraphicsContext::Create()->Init();

  RenderCommand::Init();

  shader_ =
      Shader::Create(std::filesystem::path(".eve/shaders/default.vert.spirv"),
                     std::filesystem::path(".eve/shaders/default.frag.spirv"));

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
                         {ShaderDataType::kFloat3, "a_ambient"},
                         {ShaderDataType::kFloat3, "a_diffuse"},
                         {ShaderDataType::kFloat3, "a_specular"},
                         {ShaderDataType::kFloat, "a_shininess"},
                         {ShaderDataType::kFloat3, "a_normal"},
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

  light_uniform_buffer_ = UniformBuffer::Create(sizeof(LightData), 1);
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

void Renderer::Draw(const RenderPacket& packet, const Transform& transform,
                    const Ref<Texture>& texture) {
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
    vertex.position = transform.GetModelMatrix() * vertex.position;

    // set material values
    auto material = packet.material;
    vertex.ambient = material.ambient;
    vertex.specular = material.diffuse;
    vertex.diffuse = material.specular;
    vertex.shininess = material.shininess;

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

void Renderer::Draw(const Ref<Model>& model, const Transform& transform,
                    const std::optional<Material>& material) {
  PROFILE_FUNCTION();

  for (auto mesh : model->meshes) {
    if (NeedsNewBatch(mesh.indices.size())) {
      NextBatch();
    }

    Material material_in_use =
        material.has_value() ? material.value() : mesh.material;

    float texture_index = 0.0f;
    for (Vertex vertex : mesh.vertices) {
      vertex.position = transform.GetModelMatrix() * vertex.position;

      // set material values
      vertex.ambient = material_in_use.ambient;
      vertex.specular = material_in_use.diffuse;
      vertex.diffuse = material_in_use.specular;
      vertex.shininess = material_in_use.shininess;

      vertex.tex_index = texture_index;
      vertices_[vertex_count_++] = vertex;
    }

    for (const uint32_t& index : mesh.indices) {
      indices_[index_count_++] = index_offset_ + index;
    }

    index_offset_ += mesh.vertices.size();

    stats_.index_count += mesh.indices.size();
    stats_.vertex_count += mesh.vertices.size();
  }
}

void Renderer::AddLight(const DirectionalLight& light,
                        const glm::vec3& direction) {
  dir_light = light;
  dir_light.direction = direction;
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

  // TODO find better way
  dir_light = DirectionalLight{};
}

void Renderer::Flush() {
  LightData light_data;
  light_data.directional_light = dir_light;
  light_uniform_buffer_->SetData(&light_data, sizeof(LightData));

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

  shader_->Bind();
  RenderCommand::DrawIndexed(vertex_array_, index_count_);

  stats_.draw_calls++;
}

void Renderer::NextBatch() {
  Flush();
  BeginBatch();
}
