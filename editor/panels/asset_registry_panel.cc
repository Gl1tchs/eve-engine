// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/asset_registry_panel.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "asset/asset_registry.h"
#include "utils/modify_info.h"

namespace eve {

AssetRegistryPanel::AssetRegistryPanel() : Panel(true) {}

void AssetRegistryPanel::Draw() {
  AssetRegistryMap& registry = AssetRegistry::GetRegistryMap();

  if (registry.empty()) {
    return;
  }

  for (auto& [handle, asset] : registry) {
    if (!handle || !asset) {
      continue;
    }

    ImGui::PushID(handle);

    // std::string id_str = std::to_string((uint64_t)asset->handle);
    // ImGui::InputText("Handle", &id_str, ImGuiInputTextFlags_ReadOnly);

    if (ImGui::InputText("Name", &asset->name)) {
      modify_info.SetModified();
    }

    {
      auto type_string = GetAssetTypeString(asset->GetType());
      ImGui::InputText("Type", &type_string, ImGuiInputTextFlags_ReadOnly);
    }
    
    if (ImGui::InputText("Path", &asset->path)) {
      if (AssetRegistry::Reload(asset)) {
        modify_info.SetModified();
      } else {
        EVE_LOG_EDITOR_ERROR("Unable to load asset from: {}", asset->path);
      }
    }

    // TODO maybe dropdown like components
    // ImGui::SeparatorText("Metadata");
    // AssetType type = asset->GetType();
    // switch (type) {
    //   case AssetType::kTexture: {
    //     break;
    //   }
    // }

    ImGui::Separator();

    ImGui::PopID();
  }

  ImGui::Dummy(
      ImVec2(ImGui::GetWindowWidth(), ImGui::GetContentRegionAvail().y));

  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("DND_PAYLOAD_PATH")) {
      const char* asset_path = static_cast<const char*>(payload->Data);

      if (AssetHandle handle = AssetRegistry::Load(asset_path); handle) {
        modify_info.SetModified();
      } else {
        EVE_LOG_EDITOR_ERROR("Unable to load asset from: {}", asset_path);
      }
    }

    ImGui::EndDragDropTarget();
  }
}

}  // namespace eve
