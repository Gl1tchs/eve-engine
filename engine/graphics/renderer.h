// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

#include "core/math/transform.h"
#include "core/utils/memory.h"
#include "graphics/index_buffer.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/uniform_buffer.h"
#include "graphics/vertex_array.h"
#include "graphics/vertex_buffer.h"

static constexpr size_t kMaxVertexCount = 4000;
static constexpr size_t kMaxIndexCount = 8000;
static constexpr size_t kMaxTextures = 32;

struct Vertex final {
  glm::vec4 position;
  glm::vec4 color;
  glm::vec2 tex_coords;
  float tex_index = 0.0f;
};

struct RenderPacket final {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
};

struct CameraData final {
  glm::mat4 view;
  glm::mat4 proj;
};

struct RenderStats {
  uint32_t draw_calls = 0;
  uint32_t vertex_count = 0;
  uint32_t index_count = 0;
};

class Renderer final {
 public:
  Renderer();
  ~Renderer();

  void BeginScene(const CameraData& camera_data);

  void EndScene();

  void Draw(const RenderPacket& packet);

  void Draw(const RenderPacket& packet, const Ref<Texture>& texture);

  [[nodiscard]] const RenderStats& GetStats() const { return stats_; }

  void ResetStats();

 private:
  bool NeedsNewBatch(uint32_t index_count);

  void BeginBatch();

  void Flush();

  void NextBatch();

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<IndexBuffer> index_buffer_;

  Ref<Shader> shader_;

  Vertex* vertices_{nullptr};
  uint32_t vertex_count_{0};

  uint32_t* indices_{nullptr};
  uint32_t index_count_{0};
  uint32_t index_offset_{0};

  Ref<Texture> white_texture_;
  std::array<Ref<Texture>, kMaxTextures> texture_slots_{};
  uint32_t texture_slot_index_{0};

  Ref<UniformBuffer> camera_uniform_buffer_;

  RenderStats stats_;
};