// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/event/key_code.h"
#include "core/event/mouse_code.h"

namespace eve {

class Input {
 public:
  static void Init();

  static void Reset();

  static bool IsKeyPressed(KeyCode key);

  static bool IsKeyPressed(const std::string& key);

  static bool IsKeyReleased(KeyCode key);

  static bool IsKeyReleased(const std::string& key);

  static bool IsMouseButtonPressed(MouseCode button);

  static bool IsMouseButtonPressed(const std::string& key);

  static bool IsMouseButtonReleased(MouseCode button);

  static bool IsMouseButtonReleased(const std::string& key);

  static glm::dvec2 GetMousePosition();

  static void RegisterKey(const std::string& key, KeyCode code);
  static void RegisterKey(const std::string& key, MouseCode code);

  static std::vector<std::pair<std::string, KeyCode>>& GetKeyMappings() {
    return key_mappings_;
  }

  static std::vector<std::pair<std::string, MouseCode>>& GetMouseMappings() {
    return mouse_mappings_;
  }

  static void Serialize(const fs::path& path);

  static bool Deserialize(const fs::path& path);

 private:
  static std::unordered_map<KeyCode, bool> key_press_states_;
  static std::unordered_map<KeyCode, bool> key_release_states_;

  static std::unordered_map<MouseCode, bool> mouse_press_states_;
  static std::unordered_map<MouseCode, bool> mouse_release_states_;

  static glm::dvec2 mouse_position_;

  static std::vector<std::pair<std::string, KeyCode>> key_mappings_;
  static std::vector<std::pair<std::string, MouseCode>> mouse_mappings_;
};

}  // namespace eve