// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch.h"

#include "core/event/key_code.h"

#include "ui/panel.h"

namespace eve {

enum class ProjectSettingSection {
  kGeneral,
  kInput,
  kPhysics,
  kScripting,
  kShipping,
};

class ProjectSettingsPanel : public Panel {
  EVE_IMPL_PANEL("Project Settings")

 public:
  ProjectSettingsPanel();

 protected:
  void Draw() override;

 private:
  void DrawGeneralSettings();

  void DrawInputSettings();

  void DrawPhysicsSettings();

  void DrawScriptingSettings();

  void DrawShippingSettings();

 private:
  ProjectSettingSection selected_section_ = ProjectSettingSection::kGeneral;

  inline static uint32_t counter_ = 1;
};

}  // namespace eve
