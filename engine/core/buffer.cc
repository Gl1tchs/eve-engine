// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/buffer.h"

namespace eve {

Buffer::Buffer(uint64_t size) {
  Allocate(size);
}

Buffer::Buffer(const void* data, uint64_t size)
    : data((uint8_t*)data), size(size) {}

Buffer Buffer::Copy(Buffer other) {
  Buffer result(other.size);
  memcpy(result.data, other.data, other.size);
  return result;
}

void Buffer::Allocate(uint64_t size) {
  Release();

  data = (uint8_t*)malloc(size);
  this->size = size;
}

void Buffer::Release() {
  free(data);
  data = nullptr;
  size = 0;
}

ScopedBuffer::ScopedBuffer(Buffer buffer) : buffer_(buffer) {}

ScopedBuffer::ScopedBuffer(uint64_t size) : buffer_(size) {}

ScopedBuffer::~ScopedBuffer() {
  buffer_.Release();
}

uint8_t* ScopedBuffer::GetData() {
  return buffer_.data;
}

const uint64_t& ScopedBuffer::GetSize() const {
  return buffer_.size;
}

}  // namespace eve