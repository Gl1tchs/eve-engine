// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/buffer.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"

namespace eve {

struct RenderStats;

static constexpr size_t kMeshMaxVertexCount = 10000;
static constexpr size_t kMeshMaxIndexCount = 10000;
static constexpr size_t kMeshMaxTextures = 32;

struct MeshVertex final {
  glm::vec4 position;
  Color albedo;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  float diffuse_index = 0.0f;
};

struct MeshData {
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;

  Ref<Texture> diffuse_map = nullptr;
};

class MeshPrimitive {
 public:
  MeshPrimitive();
  ~MeshPrimitive();

  void Render(RenderStats& stats);

  void Reset();

  void AddInstance(const MeshData& mesh, const glm::mat4& transform,
                   const Material& material);

  [[nodiscard]] bool NeedsNewBatch(uint32_t vertex_size, uint32_t index_size);

  void SetCustomShader(Ref<ShaderInstance> custom_shader);

  void RecompileShaders();

  [[nodiscard]] float FindTexture(const Ref<Texture>& texture);

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<IndexBuffer> index_buffer_;
  Ref<Shader> shader_;

  // Render data
  BufferArray<MeshVertex> vertices_;
  BufferArray<uint32_t> indices_;
  uint32_t index_offset_ = 0;

  // Custom shader
  std::string vertex_path_;
  std::string fragment_path_;
  Ref<ShaderInstance> custom_shader_ = nullptr;

  // Textures
  Ref<Texture> white_texture_;
  std::array<Ref<Texture>, kMeshMaxTextures> texture_slots_;
  uint32_t texture_slot_index_ = 0;
};

}  // namespace eve