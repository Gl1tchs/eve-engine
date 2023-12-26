// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/entity.h"
#include "scripting/preprocessor.h"

namespace sol {
class state;

template <bool b>
class basic_reference;
using reference = basic_reference<false>;

template <typename base_t>
struct basic_environment;
using environment = basic_environment<reference>;
}  // namespace sol

class Script {
 public:
  Script(Ref<sol::state> lua, const std::string& path);

  void Reload();

  void OnStart();

  void OnUpdate(float ds);

  void OnDestroy();

  void SetSerializeDataField(std::string name, ScriptDataType value);

  [[nodiscard]] const SerializeDataMap& GetSerializeMap() {
    return serialize_fields_;
  }

 private:
  bool LoadScript();

 private:
  Entity entity_;

  Ref<sol::state> lua_;
  Scope<sol::environment> env_;

  std::string file_path_;
  ScriptPreprocessor preprocessor_;
  SerializeDataMap serialize_fields_;

  std::function<void()> on_start_;
  std::function<void(float)> on_update_;
  std::function<void()> on_destroy_;

  friend class Scene;
};
