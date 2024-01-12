// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "ui/modal.h"

namespace eve {

enum class ExitModalAnswer {
  kSaveAndExit,
  kExitWithoutSaving,
  kCancel,
};

class ExitModal : public Modal {
  EVE_IMPL_MODAL("ExitModal")

 public:
  ExitModal();
  virtual ~ExitModal() = default;

  std::function<void(ExitModalAnswer answer)> on_answer;

 protected:
  void Draw() override;
};

}  // namespace eve