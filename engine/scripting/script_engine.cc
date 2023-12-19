// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script_engine.h"

#include <luacpp/luacpp.h>

ScriptEngine::ScriptEngine() : state_(new LuaState(luaL_newstate(), true)) {}

ScriptEngine::ScriptEngine(ScriptEngine& other)
    : state_(other.state_), scripts_(other.scripts_) {}

ScriptEngine::~ScriptEngine() {
  for (Script* script : scripts_) {
    delete script;
  }

  delete state_;
}

void ScriptEngine::Start() {
  for (auto script : scripts_) {
    script->OnStart();
  }
}

void ScriptEngine::Update(float ds) {
  for (auto script : scripts_) {
    script->OnUpdate(ds);
  }
}

void ScriptEngine::Stop() {
  for (auto script : scripts_) {
    script->OnDestroy();
  }
}

Script* ScriptEngine::AddScript(const std::string& path) {
  Script* script = new Script(state_, path);
  scripts_.push_back(script);
  return script;
}
