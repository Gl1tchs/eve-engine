// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <unordered_map>

#include "core/event/key_code.h"

class Input {
 public:
  static void Init();

  static bool IsKeyPressed(KeyCode key);

  static bool IsKeyReleased(KeyCode key);

 private:
  static std::unordered_map<KeyCode, bool> key_press_states_;
  static std::unordered_map<KeyCode, bool> key_release_states_;
};