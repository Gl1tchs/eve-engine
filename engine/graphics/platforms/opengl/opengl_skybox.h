// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/skybox.h"
#include "graphics/vertex_array.h"

namespace eve {

class OpenGLSkyBox : public SkyBox {
 public:
  OpenGLSkyBox(const fs::path& path);
  OpenGLSkyBox(const std::vector<fs::path>& paths);

  virtual ~OpenGLSkyBox() = default;

  void Bind() const override;

  void UnBind() const override;

 private:
  uint32_t texture_id_;
};

}  // namespace eve
