// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/system.h"

namespace eve {

struct PhysicsSystemSettings {
  glm::vec3 gravity = {0.0f, -9.8f, 0.0f};
};

class PhysicsSystem : public System {
 public:
  PhysicsSystem();

  PhysicsSystemSettings& GetSettings() { return settings_; }

 protected:
  void OnUpdate(float ds) override;

 private:
  PhysicsSystemSettings settings_;
};

};  // namespace eve
