// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "modals/importer_modal.h"

#include <fstream>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <yaml-cpp/yaml.h>

#include "widgets/combo.h"

static AssetType DeserializeAssetType(const std::string& type);

static TextureFormat DeserializeTextureFormat(const std::string& format);

static TextureFilteringMode DeserializeTextureFilteringMode(
    const std::string mode);

static TextureWrappingMode DeserializeTextureWrappingMode(
    const std::string& mode);

ImporterModal::ImporterModal(
    std::function<void(const std::string&)> submit_callback)
    : Modal("Asset Importer", true), submit_callback_(submit_callback) {}

void ImporterModal::Draw() {
  {
    ImGui::Text("Path: ");
    ImGui::SameLine();

    ImGui::InputText("##path_input_text", &import_path_);
  }

  {
    ImGui::Text("Type");
    ImGui::SameLine();

    static const char* current_item = NULL;

    Combo::Draw("##asset_type_combo", {"Texture", "Font", "Scene", "Model"},
                [this](const std::string& selected) {
                  asset_type_ = DeserializeAssetType(selected);
                });
  }

  if (asset_type_ == AssetType::kTexture) {
    TextureMetadata metadata;

    {
      ImGui::Begin("Format:");
      ImGui::SameLine();

      Combo::Draw("##texture_format_combo",
                  {"RED", "RG", "RGB", "BGR", "RGBA", "BGRA"},
                  [&](const std::string& selected) {
                    metadata.format = DeserializeTextureFormat(selected);
                  });
    }

    const std::vector<std::string> filtering_modes = {"Nearest", "Linear"};

    {
      ImGui::Begin("Min Filter:");
      ImGui::SameLine();

      Combo::Draw("##texture_min_filter_combo", filtering_modes,
                  [&](const std::string& selected) {
                    metadata.min_filter =
                        DeserializeTextureFilteringMode(selected);
                  });
    }

    {
      ImGui::Begin("Mag Filter:");
      ImGui::SameLine();

      Combo::Draw("##texture_mag_filter_combo", filtering_modes,
                  [&](const std::string& selected) {
                    metadata.mag_filter =
                        DeserializeTextureFilteringMode(selected);
                  });
    }

    const std::vector<std::string> wrapping_modes = {
        "Repeat", "Mirrored Repeat", "Clamp to Edge", "Clamp to Border"};

    {
      ImGui::Begin("Wrap S:");
      ImGui::SameLine();

      Combo::Draw("##texture_wrap_s_combo", wrapping_modes,
                  [&](const std::string& selected) {
                    metadata.wrap_s = DeserializeTextureWrappingMode(selected);
                  });
    }

    {
      ImGui::Begin("Wrap T:");
      ImGui::SameLine();

      Combo::Draw("##texture_wrap_t_combo", wrapping_modes,
                  [&](const std::string& selected) {
                    metadata.wrap_t = DeserializeTextureWrappingMode(selected);
                  });
    }

    {
      ImGui::Begin("Generate Mipmaps:");
      ImGui::SameLine();

      ImGui::Checkbox("##generate_mipmaps_checkbox",
                      &metadata.generate_mipmaps);
    }

    texture_metadata_ = metadata;
  }

  if (ImGui::Button("Import", ImVec2(-1, 0))) {
    CreateAndWriteMeta();
    submit_callback_(GetMetaPath());
    SetActive(false);
  }
}

std::string ImporterModal::GetMetaPath() const {
  return std::format("{0}.meta", import_path_);
}

void ImporterModal::CreateAndWriteMeta() {
  std::filesystem::path meta_path_absolute =
      AssetLibrary::GetAssetPath(GetMetaPath());

  if (std::filesystem::exists(meta_path_absolute)) {
    return;
  }

  YAML::Emitter out;

  out << YAML::BeginMap;
  out << YAML::Key << "path" << YAML::Value << import_path_;
  out << YAML::Key << "type" << YAML::Value << static_cast<int>(asset_type_);

  if (asset_type_ == AssetType::kTexture) {
    out << YAML::Key << "metadata" << YAML::BeginMap;

    out << YAML::Key << "format" << YAML::Value
        << static_cast<int>(texture_metadata_.format);
    out << YAML::Key << "min_filter" << YAML::Value
        << static_cast<int>(texture_metadata_.min_filter);
    out << YAML::Key << "mag_filter" << YAML::Value
        << static_cast<int>(texture_metadata_.mag_filter);
    out << YAML::Key << "wrap_s" << YAML::Value
        << static_cast<int>(texture_metadata_.wrap_s);
    out << YAML::Key << "wrap_t" << YAML::Value
        << static_cast<int>(texture_metadata_.wrap_t);
    out << YAML::Key << "generate_mipmaps" << YAML::Value
        << static_cast<int>(texture_metadata_.generate_mipmaps);

    out << YAML::EndMap;
  }

  out << YAML::EndMap;

  std::ofstream fout(meta_path_absolute);
  fout << out.c_str();
  fout.close();
}

AssetType DeserializeAssetType(const std::string& type) {
  if (type == "Texture")
    return AssetType::kTexture;
  else if (type == "Font")
    return AssetType::kFont;
  else if (type == "Scene")
    return AssetType::kScene;
  else if (type == "Model")
    return AssetType::kStaticMesh;

  LOG_ENGINE_WARNING("Unable to deserialize asset type of: {}", type);
  return AssetType::kTexture;
}

TextureFormat DeserializeTextureFormat(const std::string& format) {
  if (format == "RED")
    return TextureFormat::kRed;
  else if (format == "RG")
    return TextureFormat::kRG;
  else if (format == "RGB")
    return TextureFormat::kRGB;
  else if (format == "BGR")
    return TextureFormat::kBGR;
  else if (format == "RGBA")
    return TextureFormat::kRGBA;
  else if (format == "BGRA")
    return TextureFormat::kBGRA;

  LOG_ENGINE_WARNING("Unable to deserialize texture format of: {}", format);
  return TextureFormat::kRGB;
}

TextureFilteringMode DeserializeTextureFilteringMode(const std::string mode) {
  if (mode == "Nearest")
    return TextureFilteringMode::kLinear;
  else if (mode == "Linear")
    return TextureFilteringMode::kNearest;

  LOG_ENGINE_WARNING("Unable to deserialize texture filtering mode of: {}",
                     mode);
  return TextureFilteringMode::kLinear;
}

TextureWrappingMode DeserializeTextureWrappingMode(const std::string& mode) {
  if (mode == "Repeat")
    return TextureWrappingMode::kRepeat;
  else if (mode == "Mirrored Repeat")
    return TextureWrappingMode::kMirroredRepeat;
  else if (mode == "Clamp to Edge")
    return TextureWrappingMode::kClampToEdge;
  else if (mode == "Clamp to Border")
    return TextureWrappingMode::kClampToBorder;

  LOG_ENGINE_WARNING("Unable to deserialize texture wrapping mode of: {}",
                     mode);
  return TextureWrappingMode::kClampToEdge;
}
