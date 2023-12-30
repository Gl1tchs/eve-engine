// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

namespace eve {
#define IMPL_PANEL(name)                         \
  [[nodiscard]] std::string GetName() override { \
    return name;                                 \
  }

class Panel {
 public:
  Panel(bool default_active = false, glm::vec2 size = {}, glm::vec2 pos = {});
  virtual ~Panel() = default;

  void Render();

  void PushStyle(int style_var, std::variant<glm::vec2, float> style);

  void SetFlags(int flags);

  void SetStatic(bool value) { is_static_ = value; }

  [[nodiscard]] bool IsActive() const { return is_active_; }
  void SetActive(bool active) { is_active_ = active; };

  [[nodiscard]] bool IsFocused() const { return is_focused_; }

  [[nodiscard]] bool IsHovered() const { return is_hovered_; }

  void SetPos(const glm::vec2& pos) { panel_pos_ = pos; }
  [[nodiscard]] const glm::vec2& GetPos() const { return panel_pos_; }

  void SetSize(const glm::vec2& size) { panel_size_ = size; }
  [[nodiscard]] const glm::vec2& GetSize() const { return panel_size_; }

 protected:
  virtual void Draw() = 0;

 private:
  void Begin();

  void End();

  [[nodiscard]] virtual std::string GetName() = 0;

 private:
  std::unordered_map<int, std::variant<glm::vec2, float>> styles_;

  int window_flags_ = 0;

  bool is_active_;
  bool is_hovered_;
  bool is_focused_;
  bool is_static_ = false;

  glm::vec2 panel_size_;
  glm::vec2 panel_pos_;
};
}  // namespace eve