// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/renderer.h"

#include "asset/asset_registry.h"
#include "graphics/primitives/mesh.h"

namespace eve {

Renderer::Renderer() {
  graphics_context_ = GraphicsContext::Create();
  graphics_context_->Init();

  RenderCommand::Init();

  // Create render datas
  mesh_data_ = CreateRef<MeshPrimitive>();
  quad_data_ = CreateRef<QuadPrimitive>();
  cube_data_ = CreateRef<CubePrimitive>();
  line_data_ = CreateRef<LinePrimitive>();

  wireframe_cube_data_ = CreateRef<CubePrimitive>();

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

void Renderer::DrawModel(const Ref<Model>& model, const Transform& transform,
                         const Material& material) {
  if (!model) {
    return;
  }

  Ref<MeshPrimitive> mesh_data =
      (material.shader == 0 || !AssetRegistry::Exists(material.shader))
          ? mesh_data_
          : AddMeshPrimitiveIfNotExists(material.shader);

  const glm::mat4 transform_matrix = transform.GetTransformMatrix();

  for (MeshData mesh : model->meshes) {
    if (!mesh_data->NeedsNewBatch(mesh.vertices.size(), mesh.indices.size())) {
      NextBatch();
    }

    mesh_data->AddInstance(mesh, transform_matrix, material);

    stats_.index_count += mesh.indices.size();
    stats_.vertex_count += mesh.vertices.size();
  }
}

void Renderer::DrawQuad(const Transform& transform, const Color& color,
                        const Ref<Texture>& texture, const glm::vec2& tiling) {
  if (quad_data_->NeedsNewBatch()) {
    NextBatch();
  }

  quad_data_->AddInstance(transform, color, texture, tiling);

  stats_.index_count += kQuadIndexCount;
  stats_.vertex_count += kQuadVertexCount;
}

void Renderer::DrawCube(const Transform& transform, const Color& color,
                        PolygonMode mode) {
  Ref<CubePrimitive> cube_data =
      (mode == PolygonMode::kLine) ? wireframe_cube_data_ : cube_data_;

  if (cube_data->NeedsNewBatch()) {
    NextBatch();
  }

  cube_data->AddInstance(transform, color);

  stats_.index_count += kCubeIndexCount;
  stats_.vertex_count += kCubeVertexCount;
}

void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1,
                        const Color& color) {
  if (line_data_->NeedsNewBatch()) {
    NextBatch();
  }

  line_data_->AddInstance(p0, p1, color);

  stats_.vertex_count += kLineVertexCount;
}

void Renderer::DrawBox(Box box, const Color& color) {
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

  quad_data_->Reset();
  cube_data_->Reset();
  line_data_->Reset();

  wireframe_cube_data_->Reset();
}

void Renderer::Flush() {
  mesh_data_->Render(stats_);

  // Render custom meshes
  for (auto& [id, mesh] : custom_meshes_) {
    mesh->Render(stats_);
  }

  quad_data_->Render(stats_);
  cube_data_->Render(stats_);
  line_data_->Render(stats_);

  RenderCommand::SetPolygonMode(PolygonMode::kLine);

  wireframe_cube_data_->Render(stats_);

  RenderCommand::SetPolygonMode(PolygonMode::kFill);
}

void Renderer::NextBatch() {
  Flush();
  BeginBatch();
}

Ref<MeshPrimitive> Renderer::AddMeshPrimitiveIfNotExists(
    AssetHandle shader_handle) {
  const auto it = std::find_if(custom_meshes_.begin(), custom_meshes_.end(),
                               [shader_handle](const auto& pair) {
                                 return pair.first == shader_handle;
                               });

  Ref<ShaderInstance> shader_instance =
      AssetRegistry::Get<ShaderInstance>(shader_handle);

  if (it == custom_meshes_.end()) {
    Ref<MeshPrimitive> mesh = CreateRef<MeshPrimitive>();
    mesh->SetCustomShader(shader_instance);
    mesh->RecompileShaders();
    custom_meshes_[shader_handle] = mesh;
    return mesh;
  }

  it->second->SetCustomShader(shader_instance);
  return it->second;
}

}  // namespace eve
