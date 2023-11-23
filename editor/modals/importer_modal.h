// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <functional>

#include "asset/asset_library.h"

#include "modals/modal.h"

class ImporterModal : public Modal {
 public:
  ImporterModal(std::function<void(const std::string&)> submit_callback,
                AssetType for_type = AssetType::kNone);
  virtual ~ImporterModal() = default;

 protected:
  void Draw() override;

 private:
  [[nodiscard]] std::string GetMetaPath() const;

  void CreateAndWriteMeta();

 private:
  std::string import_path_ = "res://";
  AssetType asset_type_;

  TextureMetadata texture_metadata_;

  std::function<void(const std::string&)> submit_callback_;
};
