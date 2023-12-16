// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/utils/guuid.h"

static std::random_device random_device;
static std::mt19937_64 engine(random_device());
static std::uniform_int_distribution<uint64_t> uniform_distribution;

GUUID::GUUID() : uuid_(uniform_distribution(engine)) {}

GUUID::GUUID(uint64_t uuid) : uuid_(uuid) {}
