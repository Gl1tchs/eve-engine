// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "graphics/platforms/opengl/opengl_skybox.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace eve {

std::vector<uint8_t> CropImage(const uint8_t* image, uint32_t image_width,
                               uint8_t channels, uint32_t x, uint32_t y,
                               uint32_t width, uint32_t height) {
  std::vector<uint8_t> cropped_data(width * height * channels);

  for (uint32_t h = 0; h < height; h++) {
    for (uint32_t w = 0; w < width; w++) {
      for (uint32_t c = 0; c < channels; c++) {
        cropped_data[(h * width + w) * channels + c] =
            image[((y + h) * image_width + (x + w)) * channels + c];
      }
    }
  }

  return cropped_data;
}

OpenGLSkyBox::OpenGLSkyBox(const fs::path& path) {
  int width, height, channels;
  uint8_t* data =
      stbi_load(path.string().c_str(), &width, &height, &channels, 0);

  if (!data) {
    stbi_image_free(data);
    EVE_LOG_ENGINE_ERROR("Cubemap texture failed to load at path: {0}",
                     path.string());
    ASSERT(false);
    return;
  }

  int internal_format;
  switch (channels) {
    case 1:
      internal_format = GL_RED;
      break;
    case 2:
      internal_format = GL_RG;
      break;
    case 3:
      internal_format = GL_RGB;
      break;
    case 4:
      internal_format = GL_RGBA;
      break;
    default:
      ASSERT(false, "Unsupported number of channels in the image");
      break;
  }

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

  float face_width = width / 4.0f;
  float face_height = height / 3.0f;

#define CREATE_SECTION(section, offset_x, offset_y)                          \
  {                                                                          \
    std::vector<uint8_t> cropped_data = CropImage(                           \
        data, width, channels, offset_x, offset_y, face_width, face_height); \
    glTexImage2D(section, 0, internal_format, (int)face_width,               \
                 (int)face_height, 0, internal_format, GL_UNSIGNED_BYTE,     \
                 cropped_data.data());                                       \
  }

  /*
  GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front
  */

  // Top
  CREATE_SECTION(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, face_width, 0);
  // Left
  CREATE_SECTION(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, face_height);
  // Front
  CREATE_SECTION(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, face_width, face_height);
  // Right
  CREATE_SECTION(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 2 * face_width, face_height);
  // Back
  CREATE_SECTION(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 3 * face_width, face_height);
  // Bottom
  CREATE_SECTION(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, face_width, 2 * face_height);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  stbi_image_free(data);
}

OpenGLSkyBox::OpenGLSkyBox(const std::vector<fs::path>& paths) {
  int width, height, channels;
  for (uint32_t i = 0; i < paths.size(); i++) {
    uint8_t* data =
        stbi_load(paths[i].string().c_str(), &width, &height, &channels, 0);

    int internal_format;
    switch (channels) {
      case 1:
        internal_format = GL_RED;
        break;
      case 2:
        internal_format = GL_RG;
        break;
      case 3:
        internal_format = GL_RGB;
        break;
      case 4:
        internal_format = GL_RGBA;
        break;
      default:
        ASSERT(false, "Unsupported number of channels in the image");
        break;
    }

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format,
                   width, height, 0, internal_format, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      EVE_LOG_ENGINE_ERROR("Cubemap texture failed to load at path: {}",
                       paths[i].string());
      stbi_image_free(data);
      ASSERT(false);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void OpenGLSkyBox::Bind() const {
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
}

void OpenGLSkyBox::UnBind() const {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}  // namespace eve
