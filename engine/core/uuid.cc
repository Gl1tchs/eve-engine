// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/uuid.h"

namespace eve {

static std::random_device random_device;
static std::mt19937_64 engine(random_device());
static std::uniform_int_distribution<uint64_t> uniform_distribution;

UUID::UUID() : uuid_(uniform_distribution(engine)) {}

UUID::UUID(const uint64_t& uuid) : uuid_(uuid) {}

UUID::UUID(uint64_t&& uuid) : uuid_(std::move(uuid)) {}

UUID& UUID::operator=(const UUID& other) {
  uuid_ = (uint64_t)other;
  return *this;
}

UUID& UUID::operator=(UUID&& other) {
  uuid_ = (uint64_t)other;
  return *this;
}

UUID& UUID::operator=(const uint64_t& other) {
  uuid_ = other;
  return *this;
}

UUID& UUID::operator=(uint64_t&& other) {
  uuid_ = other;
  return *this;
}

}  // namespace eve