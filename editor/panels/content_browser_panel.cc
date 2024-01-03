// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "panels/content_browser_panel.h"

#include <IconsFontAwesome4.h>
#include <imgui.h>

#include "project/project.h"
#include "scene/scene_manager.h"

namespace eve {

std::string GetAssetTypeString(AssetType type) {
  switch (type) {
    case AssetType::kNone:
      return "NONE";
    case AssetType::kTexture:
      return "TEXTURE";
    case AssetType::kFont:
      return "FONT";
    case AssetType::kScene:
      return "SCENE";
    case AssetType::kStaticMesh:
      return "MESH";
    case AssetType::kScript:
      return "SCRIPT";
    case AssetType::kMaterial:
      return "MATERIAL";
    case AssetType::kShader:
      return "SHADER";
    default:
      return "";
  }
}

ContentBrowserPanel::ContentBrowserPanel()
    : Panel(true), base_directory_(""), current_directory_("") {
  icon_file_ = Texture::Create("assets/branding/icon_file.png");
  icon_folder_ = Texture::Create("assets/branding/icon_folder.png");
  icon_mesh_ = Texture::Create("assets/branding/icon_mesh.png");
  icon_script_ = Texture::Create("assets/branding/icon_script.png");
  icon_texture_ = Texture::Create("assets/branding/icon_texture.png");
  icon_shader_ = Texture::Create("assets/branding/icon_shader.png");

  if (!Project::GetActive()) {
    return;
  }
}

void ContentBrowserPanel::Reload() {
  base_directory_ = Project::GetAssetDirectory();
  current_directory_ = base_directory_;

  RefreshAssetTree();
}

void ContentBrowserPanel::Draw() {
  if (current_directory_.empty() || base_directory_.empty()) {
    return;
  }

  if (ImGui::Button("assets")) {
    current_directory_ = base_directory_;
  }

  ImGui::SameLine();

  ImGui::TextUnformatted("/");

  ImGui::SameLine();

  // Render each directory in the path
  fs::path relative_path = fs::relative(current_directory_, base_directory_);

  if (current_directory_ != base_directory_) {
    for (const auto& part : relative_path) {
      if (ImGui::Button(part.string().c_str())) {
        std::string rel_string = relative_path.string();
        const std::string part_str = part.string();

        size_t pos = rel_string.find(part_str);
        if (pos != std::string::npos) {
          fs::path path_without = rel_string.substr(0, pos + part_str.length());

          // Clicking on a directory navigates into it
          current_directory_ = base_directory_ / path_without;
        }
      }
      ImGui::SameLine();

      ImGui::TextUnformatted("/");

      ImGui::SameLine();
    }
  }

  static float padding = 16.0f;
  static float thumbnail_size = 100.0f;
  float cell_size = padding + thumbnail_size;

  ImGui::SameLine(ImGui::GetContentRegionMax().x -
                  ImGui::CalcTextSize("Settings").x);

  if (ImGui::BeginPopup("Settings")) {
    ImGui::SliderFloat("Asset Size", &thumbnail_size, 1, 512);
    ImGui::EndPopup();
  }

  if (ImGui::Button("Settings")) {
    ImGui::OpenPopup("Settings");
  }

  ImGui::Separator();

  float panel_width = ImGui::GetContentRegionAvail().x;
  int column_count = (int)(panel_width / cell_size);
  if (column_count < 1) {
    column_count = 1;
  }

  ImGui::Columns(column_count, 0, false);

  int i = 0;
  static int selected_idx = -1;
  for (auto& directory_entry : fs::directory_iterator(current_directory_)) {
    const fs::path& path = directory_entry.path();
    std::string filename_str = path.filename().string();

    ImGui::PushID(filename_str.c_str());

    fs::path relative_path = fs::relative(path, Project::GetAssetDirectory());

    // Clicked somewhere else, reset selected_idx to -1
    if (!ImGui::IsItemHovered() &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      selected_idx = -1;
    }

    if (selected_idx == i) {
      ImGui::PushStyleColor(ImGuiCol_Button,
                            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    }

    ImGuiIO& io = ImGui::GetIO();
    ImFont* originalFont = io.FontDefault;

    if (const auto it = asset_paths_.find(directory_entry);
        it != asset_paths_.end()) {
      if (ImGui::ImageButton(
              (ImTextureID)GetFileIcon(it->second.type)->GetTextureID(),
              ImVec2(thumbnail_size, thumbnail_size))) {
        selected_idx = (selected_idx == i) ? -1 : i;
      }

      if (ImGui::BeginDragDropSource()) {
        std::string payload_name =
            "DND_PAYLOAD_" + GetAssetTypeString(it->second.type);
        ImGui::SetDragDropPayload(payload_name.c_str(), &it->second.handle,
                                  sizeof(AssetHandle));
        ImGui::EndDragDropSource();
      }

    } else {
      if (ImGui::ImageButton(
              (ImTextureID)(directory_entry.is_directory()
                                ? icon_folder_
                                : GetFileIcon(path.extension().string()))
                  ->GetTextureID(),
              ImVec2(thumbnail_size, thumbnail_size))) {
        selected_idx = (selected_idx == i) ? -1 : i;
      }

      if (ImGui::IsItemHovered() &&
          ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        if (directory_entry.is_directory()) {
          current_directory_ /= path.filename();
        }
        if (on_scene_open && path.extension() == ".escn") {
          on_scene_open(path);
        }
      }
    }

    ImGui::PopStyleColor();

    ImGui::TextWrapped(filename_str.c_str());

    ImGui::NextColumn();

    ImGui::PopID();

    i++;
  }

  ImGui::Columns();
}

Ref<Texture> ContentBrowserPanel::GetFileIcon(AssetType type) {
  switch (type) {
    case AssetType::kTexture:
      return icon_texture_;
    case AssetType::kStaticMesh:
      return icon_mesh_;
    case AssetType::kScript:
      return icon_script_;
    case AssetType::kShader:
      return icon_shader_;
    default:
      return icon_file_;
  }
}

Ref<Texture> ContentBrowserPanel::GetFileIcon(const std::string& extension) {
  if (extension == ".cs") {
    return icon_script_;
  } else if (extension == ".obj") {
    return icon_mesh_;
  } else if (extension == ".mp3" || extension == ".ogg" ||
             extension == ".wav" || extension == ".aac") {
    return icon_audio_;
  } else if (extension == ".png" || extension == ".jpg" ||
             extension == ".jpeg" || extension == ".webp") {
    return icon_texture_;
  } else if (extension == ".glsl" || extension == ".esh") {
    return icon_shader_;
  } else {
    return icon_file_;
  }
}

void ContentBrowserPanel::RefreshAssetTree() {
  const AssetRegistryMap& asset_registry = AssetRegistry::GetRegistryMap();

  for (const auto& [handle, asset] : asset_registry) {
    asset_paths_[AssetRegistry::GetAssetPath(asset->path)] = {asset->handle,
                                                              asset->GetType()};
  }
}

}  // namespace eve
