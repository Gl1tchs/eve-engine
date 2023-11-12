// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "core/utils/functional.h"

#include "modals/modal.h"

enum class ExitModalAnswer {
  kSaveAndExit,
  kExitWithoutSaving,
  kCancel,
};

class ExitModal : public Modal {
 public:
  ExitModal(std::function<void(ExitModalAnswer answer)> on_answer_delegate);
  virtual ~ExitModal() = default;

 protected:
  void Draw() override;

 private:
  std::function<void(ExitModalAnswer answer)> on_answer_delegate_;
};
