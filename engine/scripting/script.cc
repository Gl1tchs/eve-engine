// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script.h"

#include <luacpp/luacpp.h>

#include "asset/asset_library.h"

Script::Script(LuaState* state, const std::string& path) : state_(state) {
  auto file_path = AssetLibrary::GetAssetPath(path);

  // TODO error handling
  std::string errstr;
  if (!state_->DoFile(file_path.string().c_str(), &errstr)) {
    LOG_ERROR("Unable to load script: {}", errstr.c_str());
    ASSERT(false);
  }

  on_start_func_ = CreateScope<LuaFunction>(state_->GetFunction("OnStart"));
  on_update_func_ = CreateScope<LuaFunction>(state_->GetFunction("OnUpdate"));
  on_destroy_func_ = CreateScope<LuaFunction>(state_->GetFunction("OnDestroy"));
  // todo more functions
}

void Script::OnStart() {
  if (!on_start_func_) {
    return;
  }

  on_start_func_->Execute();
}

void Script::OnUpdate(float ds) {
  if (!on_update_func_) {
    return;
  }

  on_update_func_->Execute(nullptr, nullptr, ds);
}

void Script::OnDestroy() {
  if (!on_destroy_func_) {
    return;
  }

  on_destroy_func_->Execute();
}
