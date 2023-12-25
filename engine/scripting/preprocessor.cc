// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/preprocessor.h"

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

ScriptPreprocessor::ScriptPreprocessor(const std::string& path) : path_(path) {}

void ScriptPreprocessor::Process() {
  serialize_map_.clear();

  std::ifstream file(path_);
  if (!file.is_open()) {
    LOG_ERROR("Failed to open file: {}", path_);
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Parse serialize fields
    const auto serialize_field = ParseAnnotations(file, line);
    if (serialize_field.has_value()) {
      serialize_map_.emplace(serialize_field.value());
    }
  }

  file.close();
}

std::optional<std::pair<std::string, ScriptDataField>>
ScriptPreprocessor::ParseAnnotations(std::ifstream& file,
                                     std::string& line) const {
  std::pair<std::string, ScriptDataField> field;

  const size_t pos = line.find("--@serializable");
  if (pos == std::string::npos) {
    return {};
  }

  if (std::getline(file, line)) {
    const size_t eq_pos = line.find('=');
    if (eq_pos == std::string::npos) {
      return {};
    }

    std::string var_name = line.substr(0, eq_pos);
    var_name.erase(var_name.find_last_not_of(" \t") + 1);

    // Check if its not commented
    const size_t comment_pos = var_name.find_first_of("--");
    if (comment_pos != std::string::npos) {
      return {};
    }

    // can not serialize local variables
    const std::string substr = "local ";
    const size_t local_pos = var_name.find(substr);
    if (local_pos != std::string::npos) {
      return {};
    }

    std::string var_value = line.substr(eq_pos + 1);
    var_value.erase(0, var_value.find_first_not_of(" \t"));

    // Remove semicolon from the end of the variable value if has it
    const size_t semicolon_pos = var_value.find_last_of(';');
    if (semicolon_pos != std::string::npos) {
      var_value.erase(semicolon_pos);
    }

    // I wish c++ had pattern matching :/
    field.first = var_name;
    if (IsFloat(var_value)) {
      const float var_value_float = std::stof(var_value);
      if (IsInteger(var_value_float)) {
        field.second.value = (int)var_value_float;
      } else {
        field.second.value = var_value_float;
      }
    } else {
      field.second.value = var_value;
    }
  }

  return field;
}
