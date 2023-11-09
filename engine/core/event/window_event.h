// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_EVENT_WINDOW_EVENT_H_
#define EVE_EVENT_WINDOW_EVENT_H_

#include <glm/glm.hpp>

#include "core/event/event_handler.h"

class WindowResizeEvent final : public Event {
 public:
  explicit WindowResizeEvent(const int width, const int height)
      : size_(width, height) {}

  [[nodiscard]] glm::ivec2 GetSize() const { return size_; }

 private:
  glm::ivec2 size_;
};

class WindowCloseEvent final : public Event {};

#endif  // EVE_EVENT_WINDOW_EVENT_H_