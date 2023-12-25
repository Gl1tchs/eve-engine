// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script_engine.h"

#include <sol/sol.hpp>

#include "asset/asset_library.h"
#include "core/utils/memory.h"
#include "scripting/engine_api.h"

sol::state* ScriptEngine::lua_ = nullptr;

void ScriptEngine::Init() {
  lua_ = new sol::state();
  lua_->open_libraries(sol::lib::base);

  RegisterTypes(lua_);
}

void ScriptEngine::Deinit() {
  delete lua_;
}

Ref<Script> ScriptEngine::CreateScript(const std::string& path) {
  std::string path_abs = AssetLibrary::GetAssetPath(path).string();
  if (!std::filesystem::exists(path_abs)) {
    LOG_WARNING("Unable to load script from: {}", path_abs);
    return nullptr;
  }

  Ref<Script> script = CreateRef<Script>(lua_, path_abs);
  return script;
}
