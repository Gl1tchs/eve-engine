// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/primitives/primitive.h"

#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"

namespace eve {

constexpr size_t kQuadVertexCount = 4;
constexpr size_t kQuadIndexCount = 6;

constexpr size_t kQuadMaxInstances = 1000;

constexpr size_t kQuadMaxVertexCount = kQuadMaxInstances * kQuadVertexCount;
constexpr size_t kQuadMaxIndexCount = kQuadMaxInstances * kQuadIndexCount;
constexpr size_t kQuadMaxTextures = 32;

struct QuadVertex final {
  glm::vec4 position;
  glm::vec2 tex_coords;
  Color color;
  float tex_index = 0.0f;
  glm::vec2 tex_tiling = {1, 1};
  glm::vec2 tex_offset = {0, 0};
};

const glm::vec4 kQuadVertexPositions[kQuadVertexCount] = {
    {-0.5f, -0.5f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f}};

const glm::vec2 kQuadVertexTexCoords[kQuadVertexCount] = {{0.0f, 0.0f},
                                                          {0.0f, 0.5f},
                                                          {0.5f, 0.5f},
                                                          {0.5f, 0.0f}};

template <>
struct RenderData<QuadVertex> {
  std::string name;
  std::vector<QuadVertex> vertices;
  std::vector<uint32_t> indices;
  Ref<Texture> texture;
};

class QuadPrimitive : public Primitive<QuadVertex> {
 public:
  QuadPrimitive();

  virtual ~QuadPrimitive() = default;

  void Render(RenderStats& stats) override;

  [[nodiscard]] bool NeedsNewBatch() override;

  [[nodiscard]] float FindTexture(const Ref<Texture>& texture);

  void IncrementIndex(const uint32_t& count = 6) { index_count_ += count; }

 protected:
  void OnReset() override;

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;

  uint32_t index_count_;

  Ref<Shader> shader_;
  std::string vertex_path_;
  std::string fragment_path_;

  // Textures
  Ref<Texture> white_texture_;
  std::array<Ref<Texture>, kQuadMaxTextures> texture_slots_;
  uint32_t texture_slot_index_;
};
}  // namespace eve