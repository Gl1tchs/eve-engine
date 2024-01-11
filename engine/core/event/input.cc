// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/event/input.h"

#include <nlohmann/json.hpp>

#include "core/event/event_handler.h"
#include "core/event/key_event.h"
#include "core/event/mouse_event.h"

using json = nlohmann::json;

namespace eve {

std::unordered_map<KeyCode, bool> Input::key_press_states_ = {};
std::unordered_map<KeyCode, bool> Input::key_release_states_ = {};
std::unordered_map<MouseCode, bool> Input::mouse_press_states_ = {};
std::unordered_map<MouseCode, bool> Input::mouse_release_states_ = {};
glm::dvec2 Input::mouse_position_ = glm::dvec2(0.0f);

std::vector<std::pair<std::string, KeyCode>> Input::key_mappings_ = {};
std::vector<std::pair<std::string, MouseCode>> Input::mouse_mappings_ = {};

void Input::Init() {
  Reset();

  SubscribeEvent<KeyPressEvent>([&](const KeyPressEvent& event) {
    key_press_states_[event.GetKeyCode()] = true;
    key_release_states_[event.GetKeyCode()] = false;
  });

  SubscribeEvent<KeyReleaseEvent>([&](const KeyReleaseEvent& event) {
    key_press_states_[event.GetKeyCode()] = false;
    key_release_states_[event.GetKeyCode()] = true;
  });

  SubscribeEvent<MouseMoveEvent>([&](const MouseMoveEvent& event) {
    mouse_position_ = event.GetPosition();
  });

  SubscribeEvent<MouseButtonPressEvent>(
      [&](const MouseButtonPressEvent& event) {
        mouse_press_states_[event.GetButtonCode()] = true;
        mouse_release_states_[event.GetButtonCode()] = false;
      });

  SubscribeEvent<MouseButtonReleaseEvent>(
      [&](const MouseButtonReleaseEvent& event) {
        mouse_press_states_[event.GetButtonCode()] = false;
        mouse_release_states_[event.GetButtonCode()] = true;
      });
}

void Input::Reset() {
  event_callbacks<KeyPressEvent>.clear();
  event_callbacks<KeyReleaseEvent>.clear();
  event_callbacks<KeyTypeEvent>.clear();
  event_callbacks<MouseButtonPressEvent>.clear();
  event_callbacks<MouseButtonReleaseEvent>.clear();
  event_callbacks<MouseMoveEvent>.clear();
  event_callbacks<MouseScrollEvent>.clear();

  key_press_states_.clear();
  key_release_states_.clear();
  mouse_press_states_.clear();
  mouse_release_states_.clear();
  mouse_position_ = glm::dvec2(0.0f);
  key_mappings_.clear();
  mouse_mappings_.clear();
}

bool Input::IsKeyPressed(KeyCode key) {
  auto it = key_press_states_.find(key);
  if (it != key_press_states_.end()) {
    return it->second;
  }
  return false;
}

bool Input::IsKeyPressed(const std::string& key) {
  const auto it =
      std::find_if(key_mappings_.begin(), key_mappings_.end(),
                   [key](const auto& pair) { return pair.first == key; });

  if (it == key_mappings_.end()) {
    LOG_ENGINE_WARNING("KeyMapping for '{}' not found!", key);
    return false;
  }

  return IsKeyPressed(it->second);
}

bool Input::IsKeyReleased(KeyCode key) {
  auto it = key_release_states_.find(key);
  if (it != key_release_states_.end()) {
    return it->second;
  }
  return false;
}

bool Input::IsKeyReleased(const std::string& key) {
  const auto it =
      std::find_if(key_mappings_.begin(), key_mappings_.end(),
                   [key](const auto& pair) { return pair.first == key; });

  if (it == key_mappings_.end()) {
    LOG_ENGINE_WARNING("KeyMapping for '{}' not found!", key);
    return false;
  }

  return IsKeyReleased(it->second);
}

bool Input::IsMouseButtonPressed(MouseCode button) {
  auto it = mouse_press_states_.find(button);
  if (it != mouse_press_states_.end()) {
    return it->second;
  }
  return false;
}

bool Input::IsMouseButtonPressed(const std::string& key) {
  const auto it =
      std::find_if(mouse_mappings_.begin(), mouse_mappings_.end(),
                   [key](const auto& pair) { return pair.first == key; });

  if (it == mouse_mappings_.end()) {
    LOG_ENGINE_WARNING("KeyMapping for '{}' not found!", key);
    return false;
  }

  return IsMouseButtonPressed(it->second);
}

bool Input::IsMouseButtonReleased(MouseCode button) {
  auto it = mouse_release_states_.find(button);
  if (it != mouse_release_states_.end()) {
    return it->second;
  }
  return false;
}

bool Input::IsMouseButtonReleased(const std::string& key) {
  const auto it =
      std::find_if(mouse_mappings_.begin(), mouse_mappings_.end(),
                   [key](const auto& pair) { return pair.first == key; });

  if (it == mouse_mappings_.end()) {
    LOG_ENGINE_WARNING("KeyMapping for '{}' not found!", key);
    return false;
  }

  return IsMouseButtonReleased(it->second);
}

glm::dvec2 Input::GetMousePosition() {
  return mouse_position_;
}

void Input::RegisterKey(const std::string& key, KeyCode code) {
  auto it = std::find_if(key_mappings_.begin(), key_mappings_.end(),
                         [key](const auto& pair) { return pair.first == key; });

  if (it != key_mappings_.end()) {
    it->second = code;
  }

  key_mappings_.push_back(std::make_pair(key, code));
}

void Input::RegisterKey(const std::string& key, MouseCode code) {
  auto it = std::find_if(mouse_mappings_.begin(), mouse_mappings_.end(),
                         [key](const auto& pair) { return pair.first == key; });

  if (it != mouse_mappings_.end()) {
    it->second = code;
  }

  mouse_mappings_.push_back(std::make_pair(key, code));
}

void Input::Serialize(const fs::path& path) {
  json j{{"key_mappings", json::array()}, {"mouse_mappings", json::array()}};

  for (const auto& [key, code] : key_mappings_) {
    j["key_mappings"].push_back(
        json{{"key", key}, {"code", static_cast<uint32_t>(code)}});
  }

  json mouse_maps = json::array();
  for (const auto& [key, code] : mouse_mappings_) {
    j["mouse_mappings"].push_back(
        json{{"key", key}, {"code", static_cast<uint32_t>(code)}});
  }

  std::ofstream fout(path);
  fout << j.dump(2);
}

bool Input::Deserialize(const fs::path& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    LOG_ENGINE_ERROR("Failed to deserialize input mappings from: {}",
                     path.string());
    return false;
  }

  json j;
  file >> j;

  auto key_mappings_json = j["key_mappings"];
  for (auto& mapping_json : key_mappings_json) {
    key_mappings_.push_back(std::make_pair(
        mapping_json["key"].get<std::string>(),
        static_cast<KeyCode>(mapping_json["code"].get<uint32_t>())));
  }

  auto mouse_mappings_json = j["mouse_mappings"];
  for (auto& mapping_json : mouse_mappings_json) {
    mouse_mappings_.push_back(std::make_pair(
        mapping_json["key"].get<std::string>(),
        static_cast<MouseCode>(mapping_json["code"].get<uint32_t>())));
  }

  return true;
}

}  // namespace eve