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

  // Initialize shape render data
  {
    shape_data_.vertex_array_ = VertexArray::Create();

    shape_data_.shader_ = Shader::Create(
        std::filesystem::path(".eve/shaders/default.vert.spirv"),
        std::filesystem::path(".eve/shaders/default.frag.spirv"));

    shape_data_.shader_->Bind();

    int samplers[32];
    // fill the textures with empty values
    std::iota(std::begin(samplers), std::end(samplers), 0);
    shape_data_.shader_->SetUniform("u_textures", 32, samplers);

    // initialize vertex buffer
    shape_data_.vertices = new Vertex[kMaxVertexCount];
    shape_data_.vertex_buffer_ =
        VertexBuffer::Create(kMaxVertexCount * sizeof(Vertex));
    shape_data_.vertex_buffer_->SetLayout({
        {ShaderDataType::kFloat4, "a_pos"},
        {ShaderDataType::kFloat3, "a_normal"},
        {ShaderDataType::kFloat2, "a_tex_coords"},
        {ShaderDataType::kFloat3, "a_ambient"},
        {ShaderDataType::kFloat3, "a_diffuse"},
        {ShaderDataType::kFloat3, "a_specular"},
        {ShaderDataType::kFloat, "a_shininess"},
        {ShaderDataType::kFloat, "a_tex_index"},
        {ShaderDataType::kInt, "a_entity_id"},
    });
    shape_data_.vertex_array_->AddVertexBuffer(shape_data_.vertex_buffer_);

    // initialize index buffer
    shape_data_.indices_ = new uint32_t[kMaxIndexCount];
    shape_data_.index_buffer_ =
        IndexBuffer::Create(kMaxIndexCount * sizeof(uint32_t));
    shape_data_.vertex_array_->SetIndexBuffer(shape_data_.index_buffer_);

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
    shape_data_.white_texture_ = Texture::Create(metadata, &color);

    // Fill texture slots with default white texture
    std::fill(std::begin(shape_data_.texture_slots_),
              std::end(shape_data_.texture_slots_), shape_data_.white_texture_);
  }

  // Initialize line data
  {
    line_data_.vertex_array_ = VertexArray::Create();

    line_data_.shader_ =
        Shader::Create(std::filesystem::path(".eve/shaders/line.vert.spirv"),
                       std::filesystem::path(".eve/shaders/line.frag.spirv"));

    line_data_.vertices = new LineVertex[kMaxVertexCount];
    line_data_.vertex_buffer_ =
        VertexBuffer::Create(kMaxVertexCount * sizeof(LineVertex));
    line_data_.vertex_buffer_->SetLayout({
        {ShaderDataType::kFloat3, "a_pos"},
        {ShaderDataType::kFloat4, "a_color"},
        {ShaderDataType::kInt, "a_entity_id"},
    });
    line_data_.vertex_array_->AddVertexBuffer(line_data_.vertex_buffer_);
  }

  camera_uniform_buffer_ = UniformBuffer::Create(sizeof(CameraData), 0);
  light_uniform_buffer_ = UniformBuffer::Create(sizeof(LightData), 1);
}

Renderer::~Renderer() {
  PROFILE_FUNCTION();

  delete[] shape_data_.vertices;
  delete[] shape_data_.indices_;

  delete[] line_data_.vertices;
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
                    const Ref<Texture>& texture, int entity_id) {
  PROFILE_FUNCTION();

  if (NeedsNewBatch(shape_data_.index_count_, packet.indices.size()) ||
      shape_data_.texture_slot_index_ >= kMaxTextures) {
    NextBatch();
  }

  float texture_index = 0.0f;
  for (uint32_t i = 1; i < shape_data_.texture_slot_index_; i++) {
    if (shape_data_.texture_slots_[i] == texture) {
      texture_index = (float)i;
    }
  }

  if (texture_index == 0.0f) {
    texture_index = (float)shape_data_.texture_slot_index_;
    shape_data_.texture_slots_[shape_data_.texture_slot_index_++] = texture;
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
    vertex.entity_id = entity_id;

    shape_data_.vertices[shape_data_.vertex_count_++] = vertex;
  }

  for (const uint32_t& index : packet.indices) {
    shape_data_.indices_[shape_data_.index_count_++] =
        shape_data_.index_offset_ + index;
  }

  shape_data_.index_offset_ += packet.vertices.size();

  stats_.index_count += packet.indices.size();
  stats_.vertex_count += packet.vertices.size();
}

void Renderer::Draw(const Ref<Model>& model, const Transform& transform,
                    const std::optional<Material>& material, int entity_id) {
  PROFILE_FUNCTION();

  for (auto mesh : model->meshes) {
    if (NeedsNewBatch(shape_data_.index_count_, mesh.indices.size())) {
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
      vertex.entity_id = entity_id;

      shape_data_.vertices[shape_data_.vertex_count_++] = vertex;
    }

    for (const uint32_t& index : mesh.indices) {
      shape_data_.indices_[shape_data_.index_count_++] =
          shape_data_.index_offset_ + index;
    }

    shape_data_.index_offset_ += mesh.vertices.size();

    stats_.index_count += mesh.indices.size();
    stats_.vertex_count += mesh.vertices.size();
  }
}

void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1,
                        const glm::vec4& color) {
  if (NeedsNewBatch(line_data_.vertex_count, 2)) {
    NextBatch();
  }

  line_data_.vertices[line_data_.vertex_count].position = p0;
  line_data_.vertices[line_data_.vertex_count].color = color;
  line_data_.vertex_count++;

  line_data_.vertices[line_data_.vertex_count].position = p1;
  line_data_.vertices[line_data_.vertex_count].color = color;
  line_data_.vertex_count++;
}

void Renderer::DrawBox(Box box, const glm::vec4& color) {
  DrawLine(box.top_left, box.top_right, color);
  DrawLine(box.top_right, box.bottom_right, color);
  DrawLine(box.bottom_right, box.bottom_left, color);
  DrawLine(box.bottom_left, box.top_left, color);
}

void Renderer::AddLight(const DirectionalLight& light,
                        const glm::vec3& direction) {
  DirectionalLight dir_light = light;
  dir_light.direction = direction;

  LightData light_data;
  light_data.directional_light = dir_light;
  light_uniform_buffer_->SetData(&light_data, sizeof(LightData));
}

void Renderer::ResetStats() {
  memset(&stats_, 0, sizeof(RenderStats));
}

bool Renderer::NeedsNewBatch(uint32_t current_count, uint32_t index_count) {
  return (current_count + index_count) >= kMaxIndexCount;
}

void Renderer::BeginBatch() {
  {
    shape_data_.vertex_count_ = 0;

    shape_data_.index_count_ = 0;
    shape_data_.index_offset_ = 0;

    shape_data_.texture_slot_index_ = 1;
  }

  { line_data_.vertex_count = 0; }
}

void Renderer::Flush() {
  if (shape_data_.index_count_) {
    shape_data_.index_buffer_->SetData(
        shape_data_.indices_, shape_data_.index_count_ * sizeof(uint32_t));
    shape_data_.vertex_buffer_->SetData(
        shape_data_.vertices, shape_data_.vertex_count_ * sizeof(Vertex));

    for (uint32_t i = 0; i <= shape_data_.texture_slot_index_; i++) {
      shape_data_.texture_slots_[i]->Bind(i);
    }

    shape_data_.shader_->Bind();
    RenderCommand::DrawIndexed(shape_data_.vertex_array_,
                               shape_data_.index_count_);

    stats_.draw_calls++;
  }

  if (line_data_.vertex_count) {
    line_data_.vertex_buffer_->SetData(
        line_data_.vertices, line_data_.vertex_count * sizeof(LineVertex));

    line_data_.shader_->Bind();
    RenderCommand::SetLineWidth(line_data_.line_width);
    RenderCommand::DrawLines(line_data_.vertex_array_, line_data_.vertex_count);

    stats_.draw_calls++;
  }
}

void Renderer::NextBatch() {
  Flush();
  BeginBatch();
}
