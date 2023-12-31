// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/math/box.h"
#include "graphics/graphics_context.h"
#include "graphics/primitives/line.h"
#include "graphics/primitives/mesh.h"
#include "graphics/texture.h"
#include "graphics/uniform_buffer.h"
#include "scene/static_mesh.h"
#include "scene/transform.h"

namespace eve {
struct CameraData final {
  glm::mat4 view;
  glm::mat4 proj;
  glm::vec3 position;
};

struct RenderStats final {
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

  void Draw(const RenderData<MeshVertex>& data, const Transform& transform,
            const Ref<Texture>& texture);

  void Draw(const Ref<Model>& model, const Transform& transform,
            const Material& material, CustomShaderComponent* custom_shader);

  void DrawLine(const glm::vec3& p0, const glm::vec3& p1,
                const glm::vec4& color);

  void DrawBox(Box box, const glm::vec4& color);

  [[nodiscard]] const RenderStats& GetStats() const { return stats_; }

  [[nodiscard]] const Ref<GraphicsContext> GetContext() const {
    return graphics_context_;
  }

  void ResetStats();

  void SetLineWidth(float width) { line_data_->line_width = width; }

  [[nodiscard]] bool CustomShadersProvided() const;

  void ResetShaderData();

  void RecompileShaders() const;

 private:
  void BeginBatch();

  void Flush();

  void NextBatch();

  Ref<MeshPrimitive> AddMeshPrimitiveIfNotExists(
      CustomShaderComponent* custom_shader);

 private:
  Ref<GraphicsContext> graphics_context_;

  // Renderer Data
  Ref<MeshPrimitive> mesh_data_;
  std::unordered_map<UUID, Ref<MeshPrimitive>> custom_meshes_;
  Scope<LinePrimitive> line_data_;

  // Camera stuff
  Ref<UniformBuffer> camera_uniform_buffer_;

  // Misc
  RenderStats stats_;
};
}  // namespace eve
