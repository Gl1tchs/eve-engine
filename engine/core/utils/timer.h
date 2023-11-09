// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <chrono>

class Timer final {
 public:
  Timer();

  void Tick();

  [[nodiscard]] float GetDeltaTime() const;

 private:
  float delta_time_;
  std::chrono::time_point<std::chrono::high_resolution_clock> last_frame_time_;
};
