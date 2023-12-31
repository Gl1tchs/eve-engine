// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

#include "core/buffer.h"

namespace eve {

class FileSystem {
 public:
  static Buffer ReadFileBinary(const fs::path& filepath);

  static std::string ReadFileString(const fs::path& filepath);
};

}  // namespace eve