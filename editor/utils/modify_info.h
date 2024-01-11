// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

namespace eve {

struct SceneModifyInfo {
  bool modified;

  void SetModified() { modified = true; }
  void OnSave() { modified = false; }
};

inline SceneModifyInfo modify_info{};

}  // namespace eve
