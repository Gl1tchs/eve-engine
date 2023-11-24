// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_frame_buffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "core/debug/assert.h"
#include "graphics/platforms/opengl/opengl_texture.h"

static constexpr uint32_t max_framebuffer_size_ = 8192;

static int TextureTarget(bool multisampled) {
  return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateTextures(bool multisampled, uint32_t* out_id,
                           uint32_t count) {
  glCreateTextures(TextureTarget(multisampled), count, out_id);
}

static void BindTexture(bool multisampled, uint32_t id) {
  glBindTexture(TextureTarget(multisampled), id);
}

static void AttachColorTexture(uint32_t id, int samples, int internal_format,
                               uint32_t format, uint32_t width, uint32_t height,
                               uint32_t index) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format,
                            width, height, GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         TextureTarget(multisampled), id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, uint32_t format,
                               uint32_t attachment_type, uint32_t width,
                               uint32_t height) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                            height, GL_FALSE);
  } else {
    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type,
                         TextureTarget(multisampled), id, 0);
}

static bool IsDepthFormat(FrameBufferTextureFormat format) {
  switch (format) {
    case FrameBufferTextureFormat::kDepth24Stencil8:
    case FrameBufferTextureFormat::kDepth32FStencil8:
      return true;
    default:
      return false;
  }
}

static int FrameBufferTextureFormatToGL(FrameBufferTextureFormat format) {
  switch (format) {
    case FrameBufferTextureFormat::kR32I:
      return GL_RED_INTEGER;
    case FrameBufferTextureFormat::kRG32F:
      return GL_RG32F;
    case FrameBufferTextureFormat::kRGB8:
      return GL_RGB8;
    case FrameBufferTextureFormat::kRGBA8:
      return GL_RGBA8;
    case FrameBufferTextureFormat::kRGBA16F:
      return GL_RGBA16F;
    case FrameBufferTextureFormat::kRGBA32F:
      return GL_RGBA32F;
    default:
      ENGINE_ASSERT(false);
      return 0;
  }
}

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
    : spec_(spec) {
  for (auto spec : spec_.attachments.attachments) {
    if (!IsDepthFormat(spec.texture_format))
      color_attachment_specs_.emplace_back(spec);
    else
      depth_attachment_spec_ = spec;
  }

  Invalidate();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
  glDeleteFramebuffers(1, &fbo_);
  glDeleteTextures(color_attachments_.size(), color_attachments_.data());
  glDeleteTextures(1, &depth_attachment_);
}

void OpenGLFrameBuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glViewport(0, 0, spec_.width, spec_.height);
}

void OpenGLFrameBuffer::Unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Invalidate() {
  if (fbo_) {
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(color_attachments_.size(), color_attachments_.data());
    glDeleteTextures(1, &depth_attachment_);

    color_attachments_.clear();
    depth_attachment_ = 0;
  }

  glCreateFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  bool multisample = spec_.samples > 1;

  if (color_attachment_specs_.size()) {
    color_attachments_.resize(color_attachment_specs_.size());

    CreateTextures(multisample, color_attachments_.data(),
                   color_attachments_.size());

    for (size_t i = 0; i < color_attachments_.size(); i++) {
      BindTexture(multisample, color_attachments_[i]);
      switch (color_attachment_specs_[i].texture_format) {
        case FrameBufferTextureFormat::kR32I:
          AttachColorTexture(color_attachments_[i], spec_.samples, GL_R32I,
                             GL_RED_INTEGER, spec_.width, spec_.height, i);
          break;
        case FrameBufferTextureFormat::kRG32F:
          AttachColorTexture(color_attachments_[i], spec_.samples, GL_RG32F,
                             GL_RG, spec_.width, spec_.height, i);
          break;
        case FrameBufferTextureFormat::kRGB8:
          AttachColorTexture(color_attachments_[i], spec_.samples, GL_RGB8,
                             GL_RGB, spec_.width, spec_.height, i);
          break;
        case FrameBufferTextureFormat::kRGBA8:
          AttachColorTexture(color_attachments_[i], spec_.samples, GL_RGBA8,
                             GL_RGBA, spec_.width, spec_.height, i);
          break;
        case FrameBufferTextureFormat::kRGBA16F:
          AttachColorTexture(color_attachments_[i], spec_.samples, GL_RGBA16F,
                             GL_RGBA, spec_.width, spec_.height, i);
          break;
        case FrameBufferTextureFormat::kRGBA32F:
          AttachColorTexture(color_attachments_[i], spec_.samples, GL_RGBA32F,
                             GL_RGBA, spec_.width, spec_.height, i);
          break;
        default:
          break;
      }
    }
  }

  if (depth_attachment_spec_.texture_format !=
      FrameBufferTextureFormat::kNone) {
    CreateTextures(multisample, &depth_attachment_, 1);
    BindTexture(multisample, depth_attachment_);
    switch (depth_attachment_spec_.texture_format) {
      case FrameBufferTextureFormat::kDepth24Stencil8:
        AttachDepthTexture(depth_attachment_, spec_.samples,
                           GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                           spec_.width, spec_.height);
        break;
      case FrameBufferTextureFormat::kDepth32FStencil8:
        AttachDepthTexture(depth_attachment_, spec_.samples,
                           GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                           spec_.width, spec_.height);
        break;
      default:
        break;
    }
  }

  if (color_attachments_.size() > 1) {
    ENGINE_ASSERT(color_attachments_.size() <= 4);
    const uint32_t buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                 GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(color_attachments_.size(), buffers);
  } else if (color_attachments_.empty()) {
    // Only depth-pass
    glDrawBuffer(GL_NONE);
  }

  ENGINE_ASSERT(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "Framebuffer is incomplete!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Resize(int w, int h) {
  if (w == 0 || h == 0 || w > max_framebuffer_size_ ||
      h > max_framebuffer_size_) {
    LOG_ENGINE_WARNING("Attempted to rezize framebuffer to {0}, {1}", w, w);
    return;
  }
  spec_.width = w;
  spec_.height = h;

  Invalidate();
}

int OpenGLFrameBuffer::ReadPixel(uint32_t index, uint32_t x, uint32_t y) {
  ENGINE_ASSERT(index < color_attachments_.size());

  glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

  int pixel_data;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);

  return pixel_data;
}

void OpenGLFrameBuffer::ClearAttachment(uint32_t attachment_index, int value) {
  ENGINE_ASSERT(attachment_index < color_attachments_.size());

  auto& spec = color_attachment_specs_[attachment_index];
  glClearTexImage(color_attachments_[attachment_index], 0,
                  FrameBufferTextureFormatToGL(spec.texture_format), GL_INT,
                  &value);
}

uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID(uint32_t index) const {
  ENGINE_ASSERT(index < color_attachments_.size());
  return color_attachments_[index];
}
