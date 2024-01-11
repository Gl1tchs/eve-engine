// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/primitives/primitive.h"

#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"

namespace eve {

constexpr size_t kCubeVertexCount = 24;
constexpr size_t kCubeIndexCount = 36;

constexpr size_t kCubeMaxInstances = 1000;

constexpr size_t kCubeMaxVertexCount = kCubeMaxInstances * kCubeVertexCount;
constexpr size_t kCubeMaxIndexCount = kCubeMaxInstances * kCubeIndexCount;
constexpr size_t kCubeMaxTextures = 32;

struct CubeVertex final {
  glm::vec4 position;
  glm::vec2 tex_coords;
  Color color;
  // float tex_index = 0.0f;
  // glm::vec2 tex_tiling = {1, 1};
  // glm::vec2 tex_offset = {0, 0};
};

const glm::vec4 kCubeVertexPositions[kCubeVertexCount] = {
    // Front face
    {-0.5f, -0.5f, 0.5f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 1.0f},
    {0.5f, 0.5f, 0.5f, 1.0f},
    {0.5f, -0.5f, 0.5f, 1.0f},

    // Back face
    {-0.5f, -0.5f, -0.5f, 1.0f},
    {-0.5f, 0.5f, -0.5f, 1.0f},
    {0.5f, 0.5f, -0.5f, 1.0f},
    {0.5f, -0.5f, -0.5f, 1.0f},

    // Left face
    {-0.5f, -0.5f, -0.5f, 1.0f},
    {-0.5f, 0.5f, -0.5f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 1.0f},

    // Right face
    {0.5f, -0.5f, -0.5f, 1.0f},
    {0.5f, 0.5f, -0.5f, 1.0f},
    {0.5f, 0.5f, 0.5f, 1.0f},
    {0.5f, -0.5f, 0.5f, 1.0f},

    // Top face
    {-0.5f, 0.5f, -0.5f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 1.0f},
    {0.5f, 0.5f, 0.5f, 1.0f},
    {0.5f, 0.5f, -0.5f, 1.0f},

    // Bottom face
    {-0.5f, -0.5f, -0.5f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 1.0f},
    {0.5f, -0.5f, 0.5f, 1.0f},
    {0.5f, -0.5f, -0.5f, 1.0f}};

const glm::vec2 kCubeVertexTexCoords[kCubeVertexCount] = {
    // Front face
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},

    // Back face
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},

    // Left face
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},

    // Right face
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},

    // Top face
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},

    // Bottom face
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f}};

template <>
struct RenderData<CubeVertex> {
  std::vector<CubeVertex> vertices;
  std::vector<uint32_t> indices;
};

class CubePrimitive : public Primitive<CubeVertex> {
 public:
  CubePrimitive();

  virtual ~CubePrimitive() = default;

  void Render(RenderStats& stats) override;

  [[nodiscard]] bool NeedsNewBatch() override;

  void IncrementIndex(const uint32_t& count = 36) { index_count_ += count; }

 protected:
  void OnReset() override;

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;

  uint32_t index_count_;

  Ref<Shader> shader_;
};

}  // namespace eve