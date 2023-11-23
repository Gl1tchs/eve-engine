// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset.h"

std::string AssetInfo::GetAssetPath() const {
  std::string path = meta_path;
  size_t last_dot_pos = path.find_last_of('.');

  // Check if the dot is found and it is not the first character in the string an then if found remove it
  if (last_dot_pos != std::string::npos && last_dot_pos != 0) {
    path = path.substr(0, last_dot_pos);
  }

  return path;
}
