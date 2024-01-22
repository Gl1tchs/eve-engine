// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/buffer.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/vertex_array.h"
#include "scene/transform.h"

namespace eve {

struct RenderStats;

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
};

class CubePrimitive {
 public:
  CubePrimitive();
  ~CubePrimitive() = default;

  void Render(RenderStats& stats);

  void Reset();

  void AddInstance(const Transform& transform, const Color& color);

  [[nodiscard]] bool NeedsNewBatch();

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<Shader> shader_;

  BufferArray<CubeVertex> vertices_;
  uint32_t index_count_ = 0;
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

}  // namespace eve