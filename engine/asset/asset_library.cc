// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "asset/asset_library.h"

#include "project/project.h"

namespace eve {

fs::path AssetLibrary::GetAssetPath(std::string relative_path) {
  std::string proj_substr = "prj://";
  std::string res_substr = "res://";

  if (auto pos = relative_path.find(proj_substr); pos != std::string::npos) {
    auto project_dir = Project::GetProjectDirectory();

    relative_path.erase(pos, proj_substr.length());

    return project_dir / relative_path;
  } else if (auto pos = relative_path.find(res_substr);
             pos != std::string::npos) {
    auto asset_dir = Project::GetAssetDirectory();

    relative_path.erase(pos, res_substr.length());

    return asset_dir / relative_path;
  }

  return relative_path;
}

}  // namespace eve