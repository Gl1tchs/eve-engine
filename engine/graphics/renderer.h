// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/math/box.h"
#include "graphics/graphics_context.h"
#include "graphics/primitives/cube.h"
#include "graphics/primitives/line.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/quad.h"
#include "graphics/render_command.h"
#include "graphics/texture.h"
#include "graphics/uniform_buffer.h"
#include "scene/model.h"
#include "scene/transform.h"

namespace eve {
struct CameraData final {
  glm::mat4 view;
  glm::mat4 proj;
  glm::vec3 position;
};

struct RenderStats final {
  float last_render_duration = 0.0f;
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

  void DrawModel(const Ref<Model>& model, const Transform& transform,
                 const Material& material = {});

  void DrawQuad(const Transform& transform, const Color& color = kColorWhite,
                const Ref<Texture>& texture = nullptr,
                const glm::vec2& tiling = {1, 1});

  void DrawCube(const Transform& transform, const Color& color,
                PolygonMode mode = PolygonMode::kFill);

  void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const Color& color);

  void DrawBox(Box box, const Color& color);

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

  Ref<MeshPrimitive> AddMeshPrimitiveIfNotExists(AssetHandle shader);

 private:
  Ref<GraphicsContext> graphics_context_;

  // Renderer datas
  Ref<MeshPrimitive> mesh_data_;
  Ref<QuadPrimitive> quad_data_;
  Ref<CubePrimitive> cube_data_;
  Ref<LinePrimitive> line_data_;

  // Wireframe renderer datas
  Ref<CubePrimitive> wireframe_cube_data_;

  // Other rendering data
  std::unordered_map<AssetHandle, Ref<MeshPrimitive>> custom_meshes_;

  // Camera stuff
  Ref<UniformBuffer> camera_uniform_buffer_;

  // Misc
  RenderStats stats_;

  friend class SceneRenderer;
};
}  // namespace eve
