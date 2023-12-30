// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {
namespace ini {

struct IniValue {
  std::string data;

  IniValue& operator=(const std::string_view& other);
  IniValue& operator=(std::string_view&& other);

  template <typename T>
  T As() const;
};

using IniValueMap = std::unordered_map<std::string, IniValue>;

struct IniSection {
  std::string name;
  IniValueMap values;

  void Add(const std::string& name, const std::string_view& value);

  IniValue& operator[](const std::string& key);

  operator bool() { return !values.empty(); }
};

struct IniObject {
  using SectionVector = std::vector<IniSection>;

  SectionVector sections;

  void AddSection(IniSection section);

  IniSection& GetSection(const std::string_view& name);

  IniValueMap& operator[](const std::string& section_name);

  [[nodiscard]] std::string ToString() const;

  operator std::string() const { return ToString(); }

  SectionVector::iterator begin() { return sections.begin(); }
  SectionVector::iterator end() { return sections.end(); }
  SectionVector::reverse_iterator rbegin() { return sections.rbegin(); }
  SectionVector::reverse_iterator rend() { return sections.rend(); }

  SectionVector::const_iterator begin() const { return sections.begin(); }
  SectionVector::const_iterator end() const { return sections.end(); }
  SectionVector::const_reverse_iterator rbegin() const {
    return sections.rbegin();
  }
  SectionVector::const_reverse_iterator rend() const { return sections.rend(); }

  operator bool() { return !sections.empty(); }
};

IniObject ParseFile(const fs::path& path);

}  // namespace ini
}  // namespace eve

#include "core/serialization/ini_parser.inl"
