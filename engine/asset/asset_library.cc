// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_library.h"

#include <algorithm>

bool AssetLibrary::Exists(GUUID id) {
  return std::find_if(
             assets_.begin(), assets_.end(),
             [id](const std::pair<std::filesystem::path, AssetInfo>& asset) {
               return asset.second.id == id;
             }) != assets_.end();
}

bool AssetLibrary::Exists(const std::string& name) {
  return std::find_if(
             assets_.begin(), assets_.end(),
             [name](const std::pair<std::filesystem::path, AssetInfo>& asset) {
               return asset.second.name == name;
             }) != assets_.end();
}
