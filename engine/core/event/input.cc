// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/event/input.h"

#include "core/event/event_handler.h"
#include "core/event/key_event.h"

std::unordered_map<KeyCode, bool> Input::key_press_states_ = {};
std::unordered_map<KeyCode, bool> Input::key_release_states_ = {};

void Input::Init() {
  SubscribeEvent<KeyPressEvent>([&](const KeyPressEvent& event) {
    key_press_states_[event.GetKeyCode()] = true;
    key_release_states_[event.GetKeyCode()] = false;
  });

  SubscribeEvent<KeyReleaseEvent>([&](const KeyReleaseEvent& event) {
    key_press_states_[event.GetKeyCode()] = false;
    key_release_states_[event.GetKeyCode()] = true;
  });
}

bool Input::IsKeyPressed(KeyCode key) {
  auto it = key_press_states_.find(key);
  if (it != key_press_states_.end()) {
    return it->second;
  }
  return false;
}

bool Input::IsKeyReleased(KeyCode key) {
  auto it = key_release_states_.find(key);
  if (it != key_release_states_.end()) {
    return it->second;
  }
  return false;
}
