// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

#include "core/math/box.h"
#include "core/utils/memory.h"
#include "graphics/index_buffer.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/uniform_buffer.h"
#include "graphics/vertex.h"
#include "graphics/vertex_array.h"
#include "graphics/vertex_buffer.h"
#include "scene/components.h"
#include "scene/lights.h"
#include "scene/transform.h"

static constexpr size_t kMaxVertexCount = 4000;
static constexpr size_t kMaxIndexCount = 8000;
static constexpr size_t kMaxTextures = 32;

struct CameraData final {
  glm::mat4 view;
  glm::mat4 proj;
  glm::vec3 position;
};

struct LightData final {
  DirectionalLight directional_light;
};

struct RenderStats final {
  uint32_t draw_calls = 0;
  uint32_t vertex_count = 0;
  uint32_t index_count = 0;
};

struct ShapeRenderData {
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<IndexBuffer> index_buffer_;
  Ref<Shader> shader_;

  Vertex* vertices{nullptr};
  uint32_t vertex_count_{0};

  uint32_t* indices_{nullptr};
  uint32_t index_count_{0};
  uint32_t index_offset_{0};

  // Textures
  Ref<Texture> white_texture_;
  std::array<Ref<Texture>, kMaxTextures> texture_slots_{};
  uint32_t texture_slot_index_{0};
};

struct LineVertex {
  glm::vec3 position;
  glm::vec4 color;
};

struct LineRenderData {
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<Shader> shader_;

  LineVertex* vertices{nullptr};
  uint32_t vertex_count{0};

  float line_width = 0.5f;
};

class Renderer final {
 public:
  Renderer();
  ~Renderer();

  void BeginScene(const CameraData& camera_data);

  void EndScene();

  void Draw(const RenderPacket& packet, const Transform& transform,
            const Ref<Texture>& texture);

  void Draw(const Ref<Model>& model, const Transform& transform,
            const std::optional<Material>& material);

  void DrawLine(const glm::vec3& p0, const glm::vec3& p1,
                const glm::vec4& color);

  void DrawBox(Box box, const glm::vec4& color);

  // TODO add multiple
  void AddLight(const DirectionalLight& light, const glm::vec3& direction);

  [[nodiscard]] const RenderStats& GetStats() const { return stats_; }

  void ResetStats();

  void SetLineWidth(float width) { line_data_.line_width = width; }

 private:
  bool NeedsNewBatch(uint32_t current_count, uint32_t index_count);

  void BeginBatch();

  void Flush();

  void NextBatch();

 private:
  // Renderer Data
  ShapeRenderData shape_data_;
  LineRenderData line_data_;

  // Camera stuff
  Ref<UniformBuffer> camera_uniform_buffer_;

  // Lightning
  Ref<UniformBuffer> light_uniform_buffer_;

  // Misc
  RenderStats stats_;
};
