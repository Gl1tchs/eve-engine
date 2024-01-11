// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "asset/asset.h"

#include "panels/panel.h"

namespace eve {
  
class AssetRegistryPanel : public Panel {
  IMPL_PANEL("Asset Registry")

 public:
  AssetRegistryPanel();

 protected:
  void Draw() override;
};

}  // namespace eve
