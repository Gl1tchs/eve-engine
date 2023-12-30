// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/file_system.h"

namespace eve {

Buffer FileSystem::ReadFileBinary(const fs::path& filepath) {
  std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

  if (!stream) {
    // Failed to open the file
    return {};
  }

  std::streampos end = stream.tellg();
  stream.seekg(0, std::ios::beg);
  uint64_t size = end - stream.tellg();

  if (size == 0) {
    // File is empty
    return {};
  }

  Buffer buffer(size);
  stream.read(buffer.As<char>(), size);
  stream.close();
  return buffer;
}

}  // namespace eve