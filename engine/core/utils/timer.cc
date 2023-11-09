// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/utils/timer.h"

Timer::Timer()
    : delta_time_(0.0f),
      last_frame_time_(std::chrono::high_resolution_clock::now()) {}

void Timer::Tick() {
  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float, std::chrono::seconds::period> delta_time =
      current_time - last_frame_time_;
  delta_time_ = delta_time.count();
  last_frame_time_ = current_time;
}

float Timer::GetDeltaTime() const {
  return delta_time_;
}
