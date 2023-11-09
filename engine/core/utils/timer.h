// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_CORE_UTILS_TIMER_H_
#define EVE_CORE_UTILS_TIMER_H_

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

#endif  // EVE_CORE_UTILS_TIMER_H_