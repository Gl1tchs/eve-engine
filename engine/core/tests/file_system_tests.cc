#include "catch2/catch_all.hpp"

#include "core/file_system.h"

using namespace eve;

TEST_CASE("FileSystem ReadFileBinary and ReadFileString", "[FileSystem]") {
  const std::string test_file_path = "test_file.txt";

  const std::string test_content = "Hello, FileSystem!";

  // Create a test file with some content
  std::ofstream test_file(test_file_path);
  test_file << test_content;
  test_file.close();

  SECTION("ReadFileBinary") {
    Buffer binary_buffer = FileSystem::ReadFileBinary(test_file_path);

    REQUIRE(binary_buffer);
    REQUIRE(binary_buffer.size > 0);

    // Check if the content of the buffer matches the expected string
    std::string content(reinterpret_cast<char*>(binary_buffer.data),
                        binary_buffer.size);
    REQUIRE(content == test_content);
  }

  SECTION("ReadFileString") {
    std::string file_content = FileSystem::ReadFileString(test_file_path);

    REQUIRE(file_content == test_content);
  }

  // Remove the test file after testing
  std::remove(test_file_path.c_str());
}