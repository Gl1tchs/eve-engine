// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>

class Modal {
 public:
  Modal(const std::string name, bool closable = true);
  virtual ~Modal() = default;

  void Render();

  [[nodiscard]] bool ShouldShow() const { return should_show_; }
  void SetActive(bool value) { should_show_ = value; }

  void SetFlags(int flags) { window_flags_ = flags; }

 protected:
  virtual void Draw() = 0;

 private:
  std::string name_;
  bool closable_;
  bool should_show_ = false;
  int window_flags_ = 0;
};
