// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_RUNTIME_INSTANCE_H_
#define EVE_RUNTIME_INSTANCE_H_

#include "core/core_minimal.h"
#include "core/window.h"

class Instance {
 public:
  Instance();
  ~Instance();

  void StartEventLoop();

 private:
  bool is_running_{true};

  Ref<Window> window_;
};

#endif  // EVE_RUNTIME_INSTANCE_H_