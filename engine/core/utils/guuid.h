// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

class GUUID {
 public:
  GUUID();
  GUUID(uint64_t uuid);
  GUUID(const GUUID&) = default;

  operator uint64_t() const { return uuid_; }

 private:
  uint64_t uuid_;
};

namespace std {
template <typename T>
struct hash;

template <>
struct hash<GUUID> {
  size_t operator()(const GUUID& uuid) const { return (uint64_t)uuid; }
};

}  // namespace std
