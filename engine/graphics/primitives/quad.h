// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/buffer.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"
#include "scene/transform.h"

namespace eve {

constexpr size_t kQuadVertexCount = 4;
constexpr size_t kQuadIndexCount = 6;

constexpr size_t kQuadMaxInstances = 1000;

constexpr size_t kQuadMaxVertexCount = kQuadMaxInstances * kQuadVertexCount;
constexpr size_t kQuadMaxIndexCount = kQuadMaxInstances * kQuadIndexCount;
constexpr size_t kQuadMaxTextures = 32;

struct RenderStats;

struct QuadVertex final {
  glm::vec4 position;
  glm::vec2 tex_coords;
  Color color;
  float tex_index = 0.0f;
  glm::vec2 tex_tiling = {1, 1};
};

class QuadPrimitive {
 public:
  QuadPrimitive();
  ~QuadPrimitive();

  void Render(RenderStats& stats);

  void Reset();

  void AddInstance(const Transform& transform, const Color& color = kColorWhite,
                   const Ref<Texture>& texture = nullptr,
                   const glm::vec2& tiling = {1, 1});

  [[nodiscard]] bool NeedsNewBatch();

  [[nodiscard]] float FindTexture(const Ref<Texture>& texture);

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<Shader> shader_;

  BufferArray<QuadVertex> vertices_;
  uint32_t index_count_ = 0;

  // Textures
  Ref<Texture> white_texture_;
  std::array<Ref<Texture>, kQuadMaxTextures> texture_slots_;
  uint32_t texture_slot_index_ = 0;
};

const glm::vec4 kQuadVertexPositions[kQuadVertexCount] = {
    {-0.5f, -0.5f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f}};

const glm::vec2 kQuadVertexTexCoords[kQuadVertexCount] = {{0.0f, 0.0f},
                                                          {0.0f, 1.0f},
                                                          {1.0f, 1.0f},
                                                          {1.0f, 0.0f}};

}  // namespace eve