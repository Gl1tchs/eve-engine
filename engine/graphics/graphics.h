// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/core_minimal.h"

#include "graphics/frame_buffer.h"
#include "graphics/graphics_context.h"
#include "graphics/index_buffer.h"
#include "graphics/renderer_api.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/uniform_buffer.h"
#include "graphics/vertex_array.h"
#include "graphics/vertex_buffer.h"

constexpr GraphicsAPI GetGraphicsAPI() {
  return GraphicsAPI::kOpenGL;
}

Ref<VertexBuffer> CreateVertexBuffer(uint32_t size);

Ref<VertexBuffer> CreateVertexBuffer(const void* vertices, uint32_t size);

Ref<IndexBuffer> CreateIndexBuffer(uint32_t size);

Ref<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count);

Ref<GraphicsContext> CreateGraphicsContext();

Ref<RendererAPI> CreateRendererAPI();

Ref<VertexArray> CreateVertexArray();

Ref<Shader> CreateShader(const std::string& vsSource,
                         const std::string& fsSource,
                         const std::string& gsSource = std::string());

Ref<Shader> CreateShader(
    const std::filesystem::path& vsPath, const std::filesystem::path& fsPath,
    const std::filesystem::path& gsPath = std::filesystem::path());

Ref<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding);

Ref<Texture> CreateTexture2D(const TextureMetadata& metadata,
                             const void* pixels = nullptr);

Ref<FrameBuffer> CreateFrameBuffer(const glm::ivec2& size);
