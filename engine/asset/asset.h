// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/uuid.h"

namespace eve {

typedef UUID AssetHandle;

enum class AssetType {
  kNone = 0,
  kTexture,
  kFont,
  kScene,
  kStaticMesh,
  kScript
};

#define IMPL_ASSET(type)               \
  AssetType GetType() const override { \
    return type;                       \
  };

struct Asset {
  AssetHandle handle;
  std::string name;
  std::string path;

  virtual AssetType GetType() const = 0;
};

}  // namespace eve