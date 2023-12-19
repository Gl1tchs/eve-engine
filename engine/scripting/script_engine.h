// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scripting/script.h"

namespace luacpp {
struct LuaState;
}

using namespace luacpp;

class ScriptEngine {
 public:
  ScriptEngine();
  ScriptEngine(ScriptEngine& other);

  ~ScriptEngine();

  void Start();

  void Update(float ds);

  void Stop();

  Script* AddScript(const std::string& path);

 private:
  LuaState* state_;

  std::vector<Script*> scripts_;
};
