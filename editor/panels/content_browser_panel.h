// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "panels/panel.h"

#include "asset/asset_registry.h"
#include "graphics/texture.h"

namespace eve {

class ContentBrowserPanel : public Panel {
  IMPL_PANEL("Content Browser")

 public:
  std::function<void(const fs::path& path)> on_scene_open;

  ContentBrowserPanel();
  virtual ~ContentBrowserPanel() = default;

  void Reload();

 protected:
  void Draw() override;

 private:
  Ref<Texture> GetFileIcon(AssetType type);

  void RefreshAssetTree();

 private:
  fs::path base_directory_;
  fs::path current_directory_;

  std::unordered_map<fs::path, std::vector<fs::path>> asset_tree;

  struct AssetData {
    AssetHandle handle;
    AssetType type;
  };
  std::unordered_map<fs::path, AssetData> asset_paths_;

  Ref<Texture> icon_file_;
  Ref<Texture> icon_folder_;
  Ref<Texture> icon_mesh_;
  Ref<Texture> icon_script_;
  Ref<Texture> icon_texture_;
  Ref<Texture> icon_audio_;
  Ref<Texture> icon_shader_;
};

}  // namespace eve
