// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/serialization/ini_parser.h"
#include "ini_parser.h"

namespace ini {

IniValue& IniValue::operator=(const std::string_view& other) {
  data = other;
  return *this;
}

IniValue& IniValue::operator=(std::string_view&& other) {
  data = other;
  return *this;
}

void IniSection::Add(const std::string& name, const std::string_view& value) {
  values[name] = value;
}

IniValue& IniSection::operator[](const std::string& key) {
  return values[key];
}

void IniObject::AddSection(IniSection section) {
  sections.push_back(section);
}

IniSection& IniObject::GetSection(const std::string_view& name) {
  const auto it = std::find_if(
      begin(), end(),
      [&name](const IniSection& section) { return name == section.name; });
  return *it;
}

IniValueMap& IniObject::operator[](const std::string& section_name) {
  // Check if the section already exists
  auto it = std::find_if(sections.begin(), sections.end(),
                         [&section_name](const IniSection& section) {
                           return section.name == section_name;
                         });

  // If the section exists, return its values
  if (it != sections.end()) {
    return it->values;
  }

  // If the section doesn't exist, create a new one and return its values
  sections.emplace_back(IniSection{section_name, IniValueMap{}});
  return sections.back().values;
}

std::string IniObject::ToString() const {
  std::stringstream ss;

  for (const auto& section : sections) {
    ss << "[" << section.name << "]\n";
    for (const auto& value : section.values) {
      ss << value.first << " = " << value.second.data << "\n";
    }
    ss << "\n";
  }

  return ss.str();
}

IniObject ParseFile(const fs::path& path) {
  static const std::regex comment_regex{R"x(\s*[;#])x"};
  static const std::regex section_regex{R"x(\s*\[([^\]]+)\])x"};
  static const std::regex value_regex{
      R"x(\s*(\S[^ \t=]*)\s*=\s*((\s?\S+)+)\s*$)x"};

  std::ifstream fin(path);
  if (!fin.is_open()) {
    return {};
  }

  IniObject obj{};

  IniSection current_section;
  std::smatch pieces;
  for (std::string line; std::getline(fin, line);) {
    if (line.empty() || std::regex_match(line, pieces, comment_regex)) {
      // skip comment lines and blank lines
    } else if (std::regex_match(line, pieces, section_regex)) {
      if (pieces.size() == 2) {  // exactly one match
        current_section.name = pieces[1].str();
      }
    } else if (std::regex_match(line, pieces, value_regex)) {
      if (pieces.size() == 4) {  // exactly enough matches
        obj[current_section.name][pieces[1].str()] = pieces[2].str();
      }
    }
  }

  return obj;
}

}  // namespace ini
