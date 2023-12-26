// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/script.h"

#include <sol/error.hpp>
#include <sol/forward.hpp>
#include <sol/load_result.hpp>
#include <sol/overload.hpp>
#include <sol/sol.hpp>
#include <sol/state_handling.hpp>
#include <sol/types.hpp>

#include "core/debug/log.h"
#include "graphics/material.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/lights.h"
#include "scene/scene.h"
#include "scene/transform.h"

Script::Script(Ref<sol::state> lua, const std::string& path)
    : lua_(lua), file_path_(path), preprocessor_(path) {
  preprocessor_.Process();
  serialize_fields_ = preprocessor_.GetSerializeMap();

  env_ = CreateScope<sol::environment>(lua_->lua_state(), sol::create,
                                       lua_->globals());
}

void Script::Reload() {
  preprocessor_.Process();
}

void Script::OnStart() {
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
  // check if key exists
  auto it = serialize_fields_.find(name);
  if (it == serialize_fields_.end()) {
    return;
  }

  it->second = {value, true};

  // Set the state to affect it immediately
  if (env_) {
    env_->set(name, value);
  }
}

template <typename T>
void RegisterComponentFunction(const std::string_view& name,
                               sol::environment& env, Entity* entity) {
  const auto kFormatCompName = [&name](const std::string_view& func_name) {
    return std::format("{}_{}", func_name, name);
  };

  env.set_function(kFormatCompName("AddComponent"), &Entity::AddComponent<T>,
                   entity);
  env.set_function(kFormatCompName("AddOrReplaceComponent"),
                   &Entity::AddOrReplaceComponent<T>, entity);
  env.set_function(kFormatCompName("GetComponent"), &Entity::GetComponent<T>,
                   entity);
  env.set_function(kFormatCompName("HasComponent"), &Entity::HasComponent<T>,
                   entity);
  env.set_function(kFormatCompName("RemoveComponent"),
                   &Entity::RemoveComponent<T>, entity);
}

void RegisterComponentFunctions(sol::environment& env, Entity* entity) {
  RegisterComponentFunction<IdComponent>("IdComponent", env, entity);
  RegisterComponentFunction<TagComponent>("TagComponent", env, entity);
  RegisterComponentFunction<Transform>("Transform", env, entity);
  RegisterComponentFunction<CameraComponent>("CameraComponent", env, entity);
  RegisterComponentFunction<Material>("Material", env, entity);
  RegisterComponentFunction<DirectionalLight>("DirectionalLight", env, entity);
}

bool Script::LoadScript() {
  if (!env_) {
    LOG_ERROR("Failed to get environment.");
    return false;
  }

  auto& env = *env_;

  env.set_function("GetTransform", &Entity::GetTransform, entity_);
  env.set_function("GetName", &Entity::GetName, entity_);
  env.set_function("GetUUID", &Entity::GetUUID, entity_);

  RegisterComponentFunctions(env, &entity_);

  const auto script_result =
      lua_->safe_script_file(file_path_, env, sol::script_pass_on_error);
  if (!script_result.valid()) {
    LOG_ERROR("Error interpretting script file at: {}\n{}", file_path_,
              ((sol::error)script_result).what());
    return false;
  }

  // Set overrided values
  for (auto& [name, data] : serialize_fields_) {
    if (data.is_overrided) {
      env.set(name, data.value);
    }
  }

  on_start_ = (sol::function)env["OnStart"];
  on_update_ = (sol::function)env["OnUpdate"];
  on_destroy_ = (sol::function)env["OnDestroy"];

  return true;
}
