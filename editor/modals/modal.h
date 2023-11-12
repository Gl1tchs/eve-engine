// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <string>

class Modal {
 public:
  Modal(const std::string name, bool closable = true);
  virtual ~Modal() = default;

  void Show();

  void SetFlags(int flags) { window_flags_ = flags; }

 protected:
  virtual void Draw() = 0;

 private:
  std::string name_;
  bool closable_;

  int window_flags_ = 0;
};
