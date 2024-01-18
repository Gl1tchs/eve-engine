// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

class Scene;

enum SystemRunType : uint16_t {
  SystemRunType_kEditor = BIT(0),
  SystemRunType_kRuntime = BIT(1),
  SystemRunType_kSimulation = BIT(2)
};

class System {
 public:
  System(uint16_t run_type);
  ~System() = default;

  [[nodiscard]] bool IsRuntime();

  [[nodiscard]] bool IsEditor();

  [[nodiscard]] bool IsSimulation();

 protected:
  virtual void OnStart() {}

  virtual void OnUpdate(float ds) {}

  virtual void OnStop() {}

  [[nodiscard]] Scene* GetScene() { return scene_; }

 private:
  Scene* scene_;
  uint16_t run_type_ = SystemRunType_kRuntime;

  friend class Scene;
};

}  // namespace eve
