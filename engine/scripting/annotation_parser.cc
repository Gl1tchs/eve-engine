// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/annotation_parser.h"

bool IsFloat(std::string str) {
  std::istringstream iss(str);
  float f;
  iss >> std::noskipws >> f;  // noskipws considers leading whitespace invalid
  // Check the entire string was consumed and if either failbit or badbit is set
  return iss.eof() && !iss.fail();
}

bool IsInteger(float value) {
  return std::floor(value) == value;
}

ScriptSerializeDataMap LuaParseAnnotations(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    LOG_ERROR("Failed to open file: {}", path);
    return {};
  }

  ScriptSerializeDataMap map{};

  std::string line;
  while (std::getline(file, line)) {
    size_t pos = line.find("--@serializable");
    if (pos == std::string::npos) {
      continue;
    }

    if (std::getline(file, line)) {
      size_t eq_pos = line.find('=');
      if (eq_pos != std::string::npos) {
        std::string var_name = line.substr(0, eq_pos);
        var_name.erase(var_name.find_last_not_of(" \t") + 1);

        std::string var_value = line.substr(eq_pos + 1);
        var_value.erase(0, var_value.find_first_not_of(" \t"));

        // Remove semicolon from the end of the variable value if has it
        size_t semicolon_pos = var_value.find_last_of(';');
        if (semicolon_pos != std::string::npos) {
          var_value.erase(semicolon_pos);
        }

        // I wish c++ had pattern matching :/
        if (IsFloat(var_value)) {
          float var_value_float = std::stof(var_value);
          if (IsInteger(var_value_float)) {
            map[var_name].value = (int)var_value_float;
          } else {
            map[var_name].value = var_value_float;
          }
        } else {
          map[var_name].value = var_value;
        }
      }
    }
  }

  file.close();

  return map;
}
