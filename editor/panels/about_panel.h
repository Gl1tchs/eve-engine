// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "panels/panel.h"

namespace eve {

class AboutPanel : public Panel {
  IMPL_PANEL("About")

 public:
  AboutPanel();

 protected:
  void Draw() override;
};

}  // namespace eve
