// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scripting/script.h"

class ScriptEngine {
 public:
  static void Init();
  static void Deinit();

  static Ref<Script> CreateScript(const std::string& path);

 private:
  static Ref<sol::state> lua_;
  static std::vector<Ref<Script>> scripts_;
};
