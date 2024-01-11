// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "graphics/shader.h"
#include "graphics/vertex_array.h"

namespace eve {

constexpr uint32_t kSkyBoxVertexCount = 36;

class SkyBox {
 public:
  virtual ~SkyBox() = default;

  void Render() const;

  virtual void Bind() const = 0;

  virtual void UnBind() const = 0;

  Ref<VertexArray> GetVertexArray() { return vertex_array_; }

  static Ref<SkyBox> Create(const fs::path& path);
  static Ref<SkyBox> Create(const std::vector<fs::path>& paths);

 protected:
  SkyBox();

 private:
  Ref<VertexArray> vertex_array_;
  Ref<VertexBuffer> vertex_buffer_;
  Ref<Shader> shader_;
};

}  // namespace eve
