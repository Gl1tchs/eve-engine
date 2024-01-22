// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/primitives/mesh.h"

#include "asset/asset_registry.h"
#include "core/file_system.h"
#include "graphics/render_command.h"
#include "graphics/renderer.h"

namespace eve {

MeshPrimitive::MeshPrimitive() {
  vertex_array_ = VertexArray::Create();

  // initialize vertex buffer
  vertices_.Allocate(kMeshMaxVertexCount);

  vertex_buffer_ = VertexBuffer::Create(vertices_.GetSize());
  vertex_buffer_->SetLayout({
      {ShaderDataType::kFloat4, "a_position"},
      {ShaderDataType::kFloat4, "a_albedo"},
      {ShaderDataType::kFloat3, "a_normal"},
      {ShaderDataType::kFloat2, "a_tex_coords"},
      {ShaderDataType::kFloat, "a_diffuse_index"},
  });
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  // initialize index buffer
  indices_.Allocate(kMeshMaxIndexCount);

  index_buffer_ = IndexBuffer::Create(indices_.GetSize());
  vertex_array_->SetIndexBuffer(index_buffer_);

  vertex_path_ = "assets/shaders/mesh.vert";
  fragment_path_ = "assets/shaders/mesh.frag";
  shader_ = Shader::Create(vertex_path_, fragment_path_);

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

MeshPrimitive::~MeshPrimitive() {}

void MeshPrimitive::Render(RenderStats& stats) {
  if (indices_.GetCount() <= 0) {
    return;
  }

  index_buffer_->SetData(indices_.GetData(), indices_.GetSize());
  vertex_buffer_->SetData(vertices_.GetData(), vertices_.GetSize());

  // Bind textures
  for (uint32_t i = 0; i <= texture_slot_index_; i++) {
    texture_slots_[i]->Bind(i);
  }

  shader_->Bind();
  if (custom_shader_) {
    for (auto& uniform : custom_shader_->uniforms) {
      shader_->SetUniform(uniform.name, uniform.value);
    }
  }

  RenderCommand::DrawIndexed(vertex_array_, indices_.GetCount());

  stats.draw_calls++;
}

void MeshPrimitive::Reset() {
  vertices_.ResetIndex();
  indices_.ResetIndex();
  index_offset_ = 0;
  texture_slot_index_ = 1;
}

void MeshPrimitive::AddInstance(const MeshData& mesh,
                                const glm::mat4& transform,
                                const Material& material) {
  float diffuse_index = FindTexture(mesh.diffuse_map);

  for (MeshVertex vertex : mesh.vertices) {
    vertex.position = transform * vertex.position;
    vertex.albedo = material.albedo;
    vertex.diffuse_index = diffuse_index;

    vertices_.Add(vertex);
  }

  for (const uint32_t& index : mesh.indices) {
    indices_.Add(index + index_offset_);
  }

  index_offset_ += mesh.vertices.size();
}

bool MeshPrimitive::NeedsNewBatch(uint32_t vertex_size, uint32_t index_size) {
  return vertices_.GetCount() + vertex_size >= kMeshMaxVertexCount ||
         indices_.GetCount() + index_size >= kMeshMaxIndexCount ||
         texture_slot_index_ + 1 >=
             kMeshMaxTextures;  // could have diffuse, specular, normal and height maps
}

void MeshPrimitive::SetCustomShader(Ref<ShaderInstance> custom_shader) {
  custom_shader_ = custom_shader;
}

void MeshPrimitive::RecompileShaders() {
  if (!custom_shader_) {
    return;
  }

  std::string custom_shader_source;
  if (!custom_shader_->path.empty()) {
    custom_shader_source = FileSystem::ReadFileString(
        AssetRegistry::GetAssetPath(custom_shader_->path).string());
  }

  shader_->Recompile(vertex_path_, fragment_path_, custom_shader_source);

  auto uniforms = custom_shader_->uniforms;
  custom_shader_->uniforms.clear();
  for (auto uniform : shader_->GetUniformFields()) {
    auto it = std::find_if(uniforms.begin(), uniforms.end(),
                           [uniform](const auto& it_uniform) {
                             return uniform.name == it_uniform.name &&
                                    uniform.type == it_uniform.type;
                           });
    if (it == uniforms.end()) {
      custom_shader_->uniforms.push_back(uniform);
      continue;
    }

    custom_shader_->uniforms.push_back(*it);
  }
}

float MeshPrimitive::FindTexture(const Ref<Texture>& texture) {
  float texture_index = 0.0f;

  if (!texture) {
    return texture_index;
  }

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
