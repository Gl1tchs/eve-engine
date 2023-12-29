// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/render_command.h"

Scope<RendererAPI> RenderCommand::renderer_api_ = nullptr;

void RenderCommand::Init() {
  renderer_api_ = RendererAPI::Create();
  renderer_api_->Init();
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t w,
                                uint32_t h) {
  renderer_api_->SetViewport(x, y, w, h);
}

void RenderCommand::SetClearColor(const glm::vec4& color) {
  renderer_api_->SetClearColor(color);
}

void RenderCommand::Clear(uint16_t bits) {
  renderer_api_->Clear(bits);
}

void RenderCommand::DrawArrays(const Ref<VertexArray>& vertex_array,
                               uint32_t vertex_count) {
  renderer_api_->DrawArrays(vertex_array, vertex_count);
}

void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertex_array,
                                uint32_t index_count) {
  renderer_api_->DrawIndexed(vertex_array, index_count);
}

void RenderCommand::DrawLines(const Ref<VertexArray>& vertex_array,
                              uint32_t vertex_count) {
  renderer_api_->DrawLines(vertex_array, vertex_count);
}

void RenderCommand::DrawArraysInstanced(const Ref<VertexArray>& vertex_array,
                                        uint32_t vertex_count,
                                        uint32_t instance_count) {
  renderer_api_->DrawArraysInstanced(vertex_array, vertex_count,
                                     instance_count);
}

void RenderCommand::SetLineWidth(float width) {
  renderer_api_->SetLineWidth(width);
}
