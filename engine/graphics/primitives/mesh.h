// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "graphics/primitives/primitive.h"

#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"

namespace eve {
static constexpr size_t kMeshMaxVertexCount = 4000;
static constexpr size_t kMeshMaxIndexCount = 8000;
static constexpr size_t kMeshMaxTextures = 32;

struct MeshVertex final {
  glm::vec4 position;
  Color albedo;
  float metallic;
  float roughness;
  float ao;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  float tex_index = 0.0f;
  glm::mat3 normal_matrix;
};

template <>
struct RenderData<MeshVertex> {
  std::string name;
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  Material material;
};

class MeshPrimitive : public Primitive<MeshVertex> {
 public:
  uint32_t index_offset;

  MeshPrimitive();

  virtual ~MeshPrimitive() = default;

  void Render(RenderStats& stats) override;

  [[nodiscard]] bool NeedsNewBatch(uint32_t vertex_size,
                                   uint32_t index_size) override;

  void AddIndex(uint32_t index);

  void Reset() override;

  void SetCustomShader(CustomShaderComponent* custom_shader);

  void RecompileShaders();

  [[nodiscard]] float FindTexture(const Ref<Texture>& texture);

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<IndexBuffer> index_buffer_;

  Ref<Shader> shader_;
  std::string vertex_path_;
  std::string fragment_path_;

  CustomShaderComponent* custom_shader_ = nullptr;

  std::vector<uint32_t> indices_;

  // Textures
  Ref<Texture> white_texture_;
  std::array<Ref<Texture>, kMeshMaxTextures> texture_slots_;
  uint32_t texture_slot_index_;
};
}  // namespace eve