// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include <imgui.h>
#include <glm/glm.hpp>

#define IMPL_PANEL(name)                         \
  [[nodiscard]] std::string GetName() override { \
    return name;                                 \
  }

struct PanelProperties {
  bool default_active;
  glm::vec2 size;
};

class Panel {
 public:
  Panel(const PanelProperties& props);
  virtual ~Panel() = default;

  void Render();

  void PushStyle(ImGuiStyleVar style_var, std::variant<ImVec2, float> style);

  void SetFlags(ImGuiWindowFlags flags);

  void SetStatic(bool value) { is_static_ = value; }

  [[nodiscard]] bool IsActive() const { return is_active_; }
  void SetActive(bool active) { is_active_ = active; };

  [[nodiscard]] const glm::vec2& GetSize() const { return panel_size_; }

 protected:
  virtual void Draw() = 0;

 private:
  void Begin();

  void End();

  [[nodiscard]] virtual std::string GetName() = 0;

 private:
  std::unordered_map<ImGuiStyleVar, std::variant<ImVec2, float>> styles_;

  ImGuiWindowFlags window_flags_ = ImGuiWindowFlags_None;

  bool is_active_;
  bool is_hovered_;
  bool is_focused_;
  bool is_static_ = false;

  glm::vec2 panel_size_;
};
