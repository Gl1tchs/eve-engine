// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/project_settings_panel.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "core/event/input.h"
#include "project/project.h"
#include "scripting/script_engine.h"

#include "utils/modify_info.h"

namespace eve {

ProjectSettingsPanel::ProjectSettingsPanel() {}

void ProjectSettingsPanel::Draw() {
  ImGui::Begin("Project Settings");

  // Sidebar
  ImGui::BeginChild("Sidebar", ImVec2(150, 0), true);

  if (ImGui::Selectable("General",
                        selected_section_ == ProjectSettingSection::kGeneral)) {
    selected_section_ = ProjectSettingSection::kGeneral;
  }
  if (ImGui::Selectable("Input",
                        selected_section_ == ProjectSettingSection::kInput)) {
    selected_section_ = ProjectSettingSection::kInput;
  }
  if (ImGui::Selectable("Physics",
                        selected_section_ == ProjectSettingSection::kPhysics)) {
    selected_section_ = ProjectSettingSection::kPhysics;
  }
  if (ImGui::Selectable("Scripting", selected_section_ ==
                                         ProjectSettingSection::kScripting)) {
    selected_section_ = ProjectSettingSection::kScripting;
  }
  if (ImGui::Selectable(
          "Shipping", selected_section_ == ProjectSettingSection::kShipping)) {
    selected_section_ = ProjectSettingSection::kShipping;
  }

  ImGui::EndChild();

  // Main content
  ImGui::SameLine();

  ImGui::BeginChild("Content", ImVec2(0, 0), true);

  switch (selected_section_) {
    case ProjectSettingSection::kGeneral:
      DrawGeneralSettings();
      break;
    case ProjectSettingSection::kInput:
      DrawInputSettings();
      break;
    case ProjectSettingSection::kPhysics:
      DrawPhysicsSettings();
      break;
    case ProjectSettingSection::kScripting:
      DrawScriptingSettings();
      break;
    case ProjectSettingSection::kShipping:
      DrawShippingSettings();
      break;
  }

  ImGui::EndChild();

  ImGui::End();
}

void ProjectSettingsPanel::DrawGeneralSettings() {
  ImGui::SeparatorText("General Settings");

  Ref<Project> project = Project::GetActive();

  auto& config = project->config_;

  if (ImGui::InputText("Name", &config.name)) {
    modify_info.SetModified();
  }
  ImGui::InputText("Asset Directory", &config.asset_directory,
                   ImGuiInputTextFlags_ReadOnly);
  if (ImGui::InputText("Asset Registry", &config.asset_registry)) {
    modify_info.SetModified();
  }
  if (ImGui::InputText("Keymaps", &config.keymaps)) {
    modify_info.SetModified();
  }

  if (ImGui::TreeNode("Scenes")) {
    for (std::string& scene : config.scenes) {
      const std::string id = std::format("##{}", scene);
      if (ImGui::InputText(id.c_str(), &scene)) {
        modify_info.SetModified();
      }
    }

    ImGui::TreePop();
  }
}

void ProjectSettingsPanel::DrawInputSettings() {
  ImGui::SeparatorText("Input Settings");

  auto& mappings = Input::GetKeyMappings();

  if (ImGui::TreeNode("KeyMaps")) {
    uint32_t i = 0;
    for (auto& [key, mapping] : mappings) {
      ImGui::Columns(2);

      ImGui::PushID(i);

      if (ImGui::InputText(
              std::format("##keymap_name{}", std::to_string(i)).c_str(),
              &key)) {
        modify_info.SetModified();
      }

      ImGui::NextColumn();

      if (ImGui::BeginCombo("##keymap_code",
                            GetKeyCodeString(mapping).c_str())) {
        for (uint32_t i = 0; i < static_cast<uint32_t>(KeyCode::kLast) + 1;
             i++) {
          const KeyCode key_code = static_cast<KeyCode>(i);
          const std::string key_code_str = GetKeyCodeString(key_code);

          // Skip none existing items
          if (key_code == KeyCode::kNone || key_code_str == "None" ||
              key_code_str.empty()) {
            continue;
          }

          const bool is_selected = (mapping == key_code);
          if (ImGui::Selectable(key_code_str.c_str(), is_selected)) {
            mapping = key_code;

            modify_info.SetModified();
          }

          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }

      ImGui::PopID();

      ImGui::Columns();

      i++;
    }

    ImGui::Separator();

    if (ImGui::Button("Add Mapping", ImVec2(-1, 0))) {
      mappings.push_back(
          std::make_pair(std::format("key{}", counter_++), KeyCode::kNone));
      modify_info.SetModified();
    }

    ImGui::TreePop();
  }
}

void ProjectSettingsPanel::DrawPhysicsSettings() {
  ImGui::SeparatorText("Physics Settings");
}

void ProjectSettingsPanel::DrawScriptingSettings() {
  ImGui::SeparatorText("Scripting Settings");

  if (ImGui::Button("Reinit", ImVec2(-1, 0))) {
    ScriptEngine::Reinit();
  }

  if (ImGui::Button("Generate Project Files", ImVec2(-1, 0))) {
    ScriptEngine::GenerateProjectFiles();
  }
}

void ProjectSettingsPanel::DrawShippingSettings() {
  ImGui::SeparatorText("Shipping Settings");
}

}  // namespace eve
