// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/event/event_handler.h"

namespace eve {

class WindowResizeEvent final : public Event {
 public:
  explicit WindowResizeEvent(const int width, const int height)
      : size_(width, height) {}

  [[nodiscard]] glm::ivec2 GetSize() const { return size_; }

 private:
  glm::ivec2 size_;
};

class WindowCloseEvent final : public Event {};

}  // namespace eve