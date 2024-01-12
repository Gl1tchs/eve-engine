// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#define EVE_IMPL_MODAL(name)                     \
  [[nodiscard]] std::string GetName() override { \
    return name;                                 \
  }

namespace eve {

class Modal {
 public:
  // Will state handled by external sources or internal
  Modal(bool external_state = false);

  virtual ~Modal() = default;

  void Render();

  [[nodiscard]] bool ShouldShow() const { return should_show_; }
  void SetActive(bool value) { should_show_ = value; }

  void SetFlags(int flags) { window_flags_ = flags; }

 protected:
  virtual void Draw() = 0;

 private:
  [[nodiscard]] virtual std::string GetName() = 0;

 private:
  bool external_state_;
  bool should_show_ = false;
  int window_flags_ = 0;
};

}  // namespace eve
