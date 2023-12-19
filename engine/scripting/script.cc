// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script.h"

#include <sol/sol.hpp>
#include <utility>

#include "scene/entity.h"
#include "scene/scene.h"
#include "scripting/annotation_parser.h"

Script::Script(sol::state* lua, const std::string& path)
    : entity_(nullptr), lua_(lua) {
  file_path_ = AssetLibrary::GetAssetPath(path).string();

  serialize_data_ = LuaParseAnnotations(file_path_);
}

void Script::OnStart() {
  sol::environment env(lua_->lua_state(), sol::create, lua_->globals());

  env.set_function("GetTransform", &Script::GetTransform, this);
  env.set_function("GetId", &Script::GetId, this);

  ASSERT(lua_->script_file(file_path_, env).valid());

  // Set overrided values
  for (auto& [name, data] : serialize_data_) {
    if (data.is_overrided) {
      auto field = env[name];
      if (field.valid()) {
        field = data.value;
      }
    }
  }

  on_start_ = (sol::function)env["OnStart"];
  on_update_ = (sol::function)env["OnUpdate"];
  on_destroy_ = (sol::function)env["OnDestroy"];

  if (!on_start_) {
    return;
  }

  on_start_();
}

void Script::OnUpdate(float ds) {
  if (!on_update_) {
    return;
  }

  on_update_(ds);
}

void Script::OnDestroy() {
  if (!on_destroy_) {
    return;
  }

  on_destroy_();
}

void Script::SetSerializeDataField(std::string name, ScriptDataType value) {
  // key doesn't exists
  auto it = serialize_data_.find(name);
  if (it == serialize_data_.end()) {
    return;
  }

  it->second = {value, true};
}

Transform& Script::GetTransform() {
  ASSERT(entity_);
  return entity_->GetTransform();
}

uint64_t Script::GetId() const {
  ASSERT(entity_);
  return (uint64_t)entity_->GetUUID();
}
