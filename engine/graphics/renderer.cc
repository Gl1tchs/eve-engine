// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/renderer.h"

#include "asset/asset_registry.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/primitive.h"

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

  for (RenderData<MeshVertex> mesh : model->meshes) {
    if (mesh_data->NeedsNewBatch(mesh.vertices.size(), mesh.indices.size())) {
      NextBatch();
    }

    const glm::mat4 model_matrix = transform.GetTransformMatrix();

    float diffuse_index = mesh_data_->FindTexture(mesh.diffuse_map);
    float specular_index = mesh_data_->FindTexture(mesh.specular_map);
    float normal_index = mesh_data_->FindTexture(mesh.normal_map);
    float height_index = mesh_data_->FindTexture(mesh.height_map);

    for (MeshVertex vertex : mesh.vertices) {
      vertex.position = model_matrix * vertex.position;
      vertex.albedo = material.albedo;

      vertex.diffuse_index = diffuse_index;
      vertex.specular_index = specular_index;
      vertex.normal_index = normal_index;
      vertex.height_index = height_index;

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

void Renderer::DrawQuad(const Transform& transform, const Color& color,
                        const glm::vec2& tiling, const glm::vec2& offset) {
  if (quad_data_->NeedsNewBatch()) {
    NextBatch();
  }

  const float tex_index = 0.0f;
  const glm::mat4 model_matrix = transform.GetTransformMatrix();

  for (size_t i = 0; i < kQuadVertexCount; i++) {
    QuadVertex vertex;

    vertex.position = model_matrix * kQuadVertexPositions[i];
    vertex.tex_coords = kQuadVertexTexCoords[i];
    vertex.color = color;
    vertex.tex_index = tex_index;
    vertex.tex_tiling = tiling;
    vertex.tex_offset = offset;

    quad_data_->AddVertex(vertex);
  }

  quad_data_->IncrementIndex();

  stats_.index_count += kQuadIndexCount;
  stats_.vertex_count += kQuadVertexCount;
}

void Renderer::DrawQuad(const Transform& transform, const Ref<Texture>& texture,
                        const Color& color, const glm::vec2& tiling,
                        const glm::vec2& offset) {
  if (quad_data_->NeedsNewBatch()) {
    NextBatch();
  }

  const float tex_index = quad_data_->FindTexture(texture);
  const glm::mat4 model_matrix = transform.GetTransformMatrix();

  for (size_t i = 0; i < kQuadVertexCount; i++) {
    QuadVertex vertex;

    vertex.position = model_matrix * kQuadVertexPositions[i];
    vertex.tex_coords = kQuadVertexTexCoords[i];
    vertex.color = color;
    vertex.tex_index = tex_index;
    vertex.tex_tiling = tiling;
    vertex.tex_offset = offset;

    quad_data_->AddVertex(vertex);
  }

  quad_data_->IncrementIndex();

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

  const glm::mat4 model_matrix = transform.GetTransformMatrix();

  for (size_t i = 0; i < kCubeVertexCount; i++) {
    CubeVertex vertex;

    vertex.position = model_matrix * kCubeVertexPositions[i];
    vertex.tex_coords = kCubeVertexTexCoords[i];
    vertex.color = color;

    cube_data->AddVertex(vertex);
  }

  cube_data->IncrementIndex();

  stats_.index_count += kCubeIndexCount;
  stats_.vertex_count += kCubeVertexCount;
}

void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1,
                        const Color& color) {
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
