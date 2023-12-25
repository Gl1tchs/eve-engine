// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

bool IsFloat(std::string str);

bool IsInteger(float value);

using ScriptDataType = std::variant<int, float, std::string>;

struct ScriptDataField {
  ScriptDataType value;
  bool is_overrided = false;
};

using SerializeDataMap = std::unordered_map<std::string, ScriptDataField>;

class ScriptPreprocessor {
 public:
  ScriptPreprocessor(const std::string& path);

  void Process();

  const SerializeDataMap& GetSerializeMap() { return serialize_map_; }

 private:
  std::string path_;
  SerializeDataMap serialize_map_;

  std::optional<std::pair<std::string, ScriptDataField>> ParseAnnotations(
      std::ifstream& file, std::string& line) const;
};
