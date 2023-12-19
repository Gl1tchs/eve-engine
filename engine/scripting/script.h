// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "scene/transform.h"
#include "scripting/annotation_parser.h"

namespace sol {
class state;
}  // namespace sol

class Entity;

class Script {
 public:
  Script(sol::state* lua, const std::string& path);

  void OnStart();

  void OnUpdate(float ds);

  void OnDestroy();

  void SetSerializeDataField(std::string name,
                                               ScriptDataType value);

  [[nodiscard]] const ScriptSerializeDataMap& GetSerializeMap() {
    return serialize_data_;
  }

 private:
  Transform& GetTransform();

  uint64_t GetId() const;

 private:
  Entity* entity_;

  sol::state* lua_;

  std::string file_path_;

  ScriptSerializeDataMap serialize_data_;

  std::function<void()> on_start_;
  std::function<void(float)> on_update_;
  std::function<void()> on_destroy_;

  friend class Scene;
};
