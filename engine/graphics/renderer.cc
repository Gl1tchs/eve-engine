// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/renderer.h"

#include "glm/matrix.hpp"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/primitive.h"
#include "graphics/render_command.h"
#include "renderer.h"

namespace eve {
Renderer::Renderer() {
  graphics_context_ = GraphicsContext::Create();
  graphics_context_->Init();

  RenderCommand::Init();

  // Create render datas
  mesh_data_ = CreateRef<MeshPrimitive>();
  line_data_ = CreateScope<LinePrimitive>();

  // Create uniform buffers
  camera_uniform_buffer_ = UniformBuffer::Create(sizeof(CameraData), 0);
}

Renderer::~Renderer() {}

void Renderer::BeginScene(const CameraData& camera_data) {
  camera_uniform_buffer_->SetData(&camera_data, sizeof(CameraData));

  BeginBatch();
}

void Renderer::EndScene() {
  Flush();
}

void Renderer::Draw(const RenderData<MeshVertex>& data,
                    const Transform& transform, const Ref<Texture>& texture) {
  if (mesh_data_->NeedsNewBatch(data.vertices.size(), data.indices.size())) {
    NextBatch();
  }

  float texture_index = mesh_data_->FindTexture(texture);

  for (MeshVertex vertex : data.vertices) {
    vertex.position = transform.GetModelMatrix() * vertex.position;

    // set material values
    auto material = data.material;
    vertex.albedo = material.albedo;
    vertex.metallic = material.metallic;
    vertex.roughness = material.roughness;
    vertex.ao = material.ao;

    vertex.tex_index = texture_index;

    mesh_data_->AddVertex(vertex);
  }

  for (const uint32_t& index : data.indices) {
    mesh_data_->AddIndex(index);
  }

  mesh_data_->index_offset += data.vertices.size();

  stats_.index_count += data.indices.size();
  stats_.vertex_count += data.vertices.size();
}

void Renderer::Draw(const Ref<Model>& model, const Transform& transform,
                    const Material& material,
                    CustomShaderComponent* custom_shader) {
  if (!model) {
    return;
  }

  Ref<MeshPrimitive> mesh_data =
      !custom_shader ? mesh_data_ : AddMeshPrimitiveIfNotExists(custom_shader);

  for (auto mesh : model->meshes) {
    if (mesh_data->NeedsNewBatch(mesh.vertices.size(), mesh.indices.size())) {
      NextBatch();
    }

    float texture_index = 0.0f;
    for (MeshVertex vertex : mesh.vertices) {
      glm::mat4 model_matrix = transform.GetModelMatrix();
      vertex.position = model_matrix * vertex.position;

      // set material values
      vertex.albedo = material.albedo;
      vertex.metallic = material.metallic;
      vertex.roughness = material.roughness;
      vertex.ao = material.ao;

      vertex.tex_index = texture_index;
      vertex.normal_matrix =
          glm::transpose(glm::inverse(glm::mat3(model_matrix)));

      mesh_data->AddVertex(vertex);
    }

    for (const uint32_t& index : mesh.indices) {
      mesh_data->AddIndex(index);
    }

    mesh_data->index_offset += mesh.vertices.size();

    stats_.index_count += mesh.indices.size();
    stats_.vertex_count += mesh.vertices.size();
  }
}

void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1,
                        const glm::vec4& color) {
  if (line_data_->NeedsNewBatch()) {
    NextBatch();
  }

  LineVertex v0;
  v0.position = p0;
  v0.color = color;
  line_data_->AddVertex(v0);

  LineVertex v1;
  v1.position = p1;
  v1.color = color;
  line_data_->AddVertex(v1);

  stats_.vertex_count += 2;
}

void Renderer::DrawBox(Box box, const glm::vec4& color) {
  DrawLine(box.top_left, box.top_right, color);
  DrawLine(box.top_right, box.bottom_right, color);
  DrawLine(box.bottom_right, box.bottom_left, color);
  DrawLine(box.bottom_left, box.top_left, color);
}

void Renderer::ResetStats() {
  memset(&stats_, 0, sizeof(RenderStats));
}

bool Renderer::CustomShadersProvided() const {
  return custom_meshes_.size() > 0;
}

void Renderer::ResetShaderData() {
  custom_meshes_.clear();
}

void Renderer::RecompileShaders() const {
  if (custom_meshes_.size() <= 0) {
    return;
  }

  for (auto& [id, mesh] : custom_meshes_) {
    mesh->RecompileShaders();
  }
}

void Renderer::BeginBatch() {
  // Reset mesh data
  mesh_data_->Reset();
  for (auto& [id, mesh] : custom_meshes_) {
    mesh->Reset();
  }

  // Reset line data
  line_data_->Reset();
}

void Renderer::Flush() {
  mesh_data_->Render(stats_);
  for (auto& [id, mesh] : custom_meshes_) {
    mesh->Render(stats_);
  }

  line_data_->Render(stats_);
}

void Renderer::NextBatch() {
  Flush();
  BeginBatch();
}

Ref<MeshPrimitive> Renderer::AddMeshPrimitiveIfNotExists(
    CustomShaderComponent* custom_shader) {
  const auto it = std::find_if(custom_meshes_.begin(), custom_meshes_.end(),
                               [custom_shader](const auto& pair) {
                                 return custom_shader->id == pair.first;
                               });
  if (it == custom_meshes_.end()) {
    Ref<MeshPrimitive> mesh = CreateRef<MeshPrimitive>();
    mesh->SetCustomShader(custom_shader);
    mesh->RecompileShaders();
    custom_meshes_[custom_shader->id] = mesh;
    return mesh;
  }

  it->second->SetCustomShader(custom_shader);
  return it->second;
}

}  // namespace eve
