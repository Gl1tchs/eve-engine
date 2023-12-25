// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/transform.h"
#include "scripting/preprocessor.h"

namespace sol {
class state;
}  // namespace sol

class Entity;

class Script {
 public:
  Script(sol::state* lua, const std::string& path);

  void Reload();

  void OnStart();

  void OnUpdate(float ds);

  void OnDestroy();

  void SetSerializeDataField(std::string name, ScriptDataType value);

  [[nodiscard]] const SerializeDataMap& GetSerializeMap() {
    return serialize_fields_;
  }

 private:
  void LoadScript();

  Transform& GetTransform();

  uint64_t GetId() const;

 private:
  Entity* entity_;

  sol::state* lua_;
  std::string file_path_;
  ScriptPreprocessor preprocessor_;
  SerializeDataMap serialize_fields_;

  std::function<void()> on_start_;
  std::function<void(float)> on_update_;
  std::function<void()> on_destroy_;

  friend class Scene;
};
