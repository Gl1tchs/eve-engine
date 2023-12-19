// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace luacpp {
struct LuaState;
class LuaFunction;
}

using namespace luacpp;

class Script {
 public:
  Script(LuaState* state, const std::string& path);

  void OnStart();

  void OnUpdate(float ds);

  void OnDestroy();

 private:
  LuaState* state_;

  Scope<LuaFunction> on_start_func_;
  Scope<LuaFunction> on_update_func_;
  Scope<LuaFunction> on_destroy_func_;
};
