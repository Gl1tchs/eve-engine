// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

namespace ini {

template <>
inline std::string IniValue::As() const {
  return data;
}

template <>
inline fs::path IniValue::As() const {
  return data;
}
}  // namespace ini
