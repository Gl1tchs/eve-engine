#include "catch2/catch_all.hpp"

#include "core/buffer.h"

using namespace eve;

TEST_CASE("Buffer Allocation and Deallocation", "[Buffer]") {
  Buffer buffer;

  buffer.Allocate(100);

  REQUIRE(buffer);
  REQUIRE(buffer.data != nullptr);
  REQUIRE(buffer.size == 100);

  buffer.Release();

  REQUIRE_FALSE(buffer);
  REQUIRE(buffer.data == nullptr);
  REQUIRE(buffer.size == 0);
}

TEST_CASE("Buffer Copy", "[Buffer]") {
  Buffer source_buffer("Hello, World!", 13);

  Buffer copied_buffer = Buffer::Copy(source_buffer);

  REQUIRE(copied_buffer.size == source_buffer.size);

  REQUIRE(memcmp(copied_buffer.data, source_buffer.data, source_buffer.size) ==
          0);
}

TEST_CASE("ScopedBuffer Construction and Destruction", "[ScopedBuffer]") {
  ScopedBuffer scoped_buffer(50);

  REQUIRE(scoped_buffer);
  REQUIRE(scoped_buffer.Data() != nullptr);
  REQUIRE(scoped_buffer.Size() == 50);
}

TEST_CASE("ScopedBuffer Type Conversion", "[ScopedBuffer]") {
  ScopedBuffer scoped_buffer(20);

  int* int_ptr = scoped_buffer.As<int>();

  REQUIRE(int_ptr != nullptr);
}
