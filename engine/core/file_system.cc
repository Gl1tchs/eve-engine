// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/file_system.h"
#include "file_system.h"

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

std::string FileSystem::ReadFileString(const fs::path& filepath) {
  std::ifstream t(filepath);
  if (!t.is_open()) {
    LOG_ERROR("Unable to load file from: {}", filepath.string());
    return "";
  }

  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}
}  // namespace eve