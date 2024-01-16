// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/skybox.h"

#include "graphics/graphics.h"
#include "graphics/render_command.h"
#include "platforms/opengl/opengl_skybox.h"

namespace eve {

const glm::vec3 kSkyBoxVertices[kSkyBoxVertexCount] = {
    {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f}};

SkyBox::SkyBox() {
  vertex_array_ = VertexArray::Create();

  vertex_buffer_ =
      VertexBuffer::Create(kSkyBoxVertices, sizeof(kSkyBoxVertices));
  vertex_buffer_->SetLayout({{ShaderDataType::kFloat3, "a_position"}});
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  shader_ = Shader::Create("assets/shaders/skybox.vert",
                           "assets/shaders/skybox.frag");

  shader_->Bind();
  shader_->SetUniform("u_skybox", 0i32);
}

void SkyBox::Render() const {
  RenderCommand::SetDepthFunc(DepthFunc::kLEqual);
  {
    RenderCommand::SetActiveTexture(0);
    this->Bind();

    shader_->Bind();
    shader_->SetUniform("u_skybox", 0i32);

    RenderCommand::DrawArrays(vertex_array_, kSkyBoxVertexCount);
  }
  RenderCommand::SetDepthFunc(DepthFunc::kLess);
}

Ref<SkyBox> SkyBox::Create(const fs::path& path) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLSkyBox>(path);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

Ref<SkyBox> SkyBox::Create(const std::vector<fs::path>& paths) {
  switch (GetGraphicsAPI()) {
    case GraphicsAPI::kOpenGL:
      return CreateRef<OpenGLSkyBox>(paths);
    case GraphicsAPI::kVulkan:
      EVE_ASSERT_ENGINE(false, "Vulkan not supported yet!");
      return nullptr;
    default:
      EVE_ASSERT_ENGINE(false, "Unknown graphics API");
      return nullptr;
  }
}

}  // namespace eve
