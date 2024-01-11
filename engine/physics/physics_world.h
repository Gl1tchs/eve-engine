// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

class Scene;

struct PhysicsWorldSettings {
  glm::vec3 gravity = {0.0f, -9.8f, 0.0f};
};

class PhysicsWorld {
 public:
  static void OnStart(Scene* scene);

  static void OnUpdate(float ds);

  static void OnStop();

  static PhysicsWorldSettings& GetSettings() { return settings_; }

 private:
  static Scene* scene_;
  static PhysicsWorldSettings settings_;
};

};  // namespace eve
