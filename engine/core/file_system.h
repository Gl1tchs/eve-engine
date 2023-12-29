// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "pch_shared.h"

#include "core/buffer.h"

class FileSystem {
 public:
  static Buffer ReadFileBinary(const fs::path& filepath);
};