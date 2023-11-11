// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <glm/glm.hpp>

#include "core/utils/memory.h"
#include "graphics/frame_buffer.h"

#include "panels/panel.h"

class ViewportPanel : public Panel {
  IMPL_PANEL("Viewport")

 public:
  ViewportPanel(Ref<FrameBuffer>& frame_buffer);

 protected:
  void Draw() override;

 private:
  Ref<FrameBuffer> frame_buffer_;
};
