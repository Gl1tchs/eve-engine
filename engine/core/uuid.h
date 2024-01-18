// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"

namespace eve {

class UUID {
 public:
  UUID();
  UUID(const uint64_t& uuid);
  UUID(uint64_t&& uuid);
  UUID(const UUID&) = default;

  UUID& operator=(const UUID& other);
  UUID& operator=(UUID&& other);

  UUID& operator=(const uint64_t& other);
  UUID& operator=(uint64_t&& other);

  [[nodiscard]] bool IsValid() const { return uuid_ != 0; }

  operator uint64_t() const { return uuid_; }

 private:
  uint64_t uuid_;
};

const UUID kInvalidUUID = UUID(0);

}  // namespace eve

namespace std {
template <typename T>
struct hash;

template <>
struct hash<eve::UUID> {
  size_t operator()(const eve::UUID& uuid) const { return (uint64_t)uuid; }
};

}  // namespace std
