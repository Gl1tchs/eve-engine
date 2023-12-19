// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

using ScriptDataType = std::variant<int, float, std::string>;

struct ScriptDataField {
  ScriptDataType value;
  bool is_overrided = false;
};

using ScriptSerializeDataMap = std::unordered_map<std::string, ScriptDataField>;

bool IsFloat(std::string str);

bool IsInteger(float value);

ScriptSerializeDataMap LuaParseAnnotations(const std::string& path);
