// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_frame_buffer.h"

#include <glad/glad.h>

#include "core/debug/log.h"
#include "graphics/platforms/opengl/opengl_texture.h"

OpenGLFrameBuffer::OpenGLFrameBuffer(const glm::ivec2& size) : size_(size) {
  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  glGenRenderbuffers(1, &rbo_);

  Refresh();

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    LOG_ENGINE_ERROR("Framebuffer is not complete!");

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
  glDeleteFramebuffers(1, &fbo_);
  glDeleteRenderbuffers(1, &rbo_);
}

void OpenGLFrameBuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void OpenGLFrameBuffer::Unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Refresh() {
  TextureMetadata metadata;
  metadata.size = size_;
  metadata.format = TextureFormat::kRGB;
  metadata.min_filter = TextureFilteringMode::kLinear;
  metadata.mag_filter = TextureFilteringMode::kLinear;
  metadata.wrap_s = TextureWrappingMode::kClampToEdge;
  metadata.wrap_t = TextureWrappingMode::kClampToEdge;
  metadata.generate_mipmaps = false;

  texture_ = CreateRef<OpenGLTexture2D>(metadata);

  glBindTexture(GL_TEXTURE_2D, texture_->GetTextureID());
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_->GetTextureID(), 0);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size_.x, size_.y);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo_);
}

const glm::ivec2& OpenGLFrameBuffer::GetSize() {
  return size_;
}

void OpenGLFrameBuffer::SetSize(glm::ivec2 size) {
  size_ = size;
}

Ref<Texture> OpenGLFrameBuffer::GetTexture() {
  return texture_;
}
