// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

struct Buffer {
  uint8_t* data = nullptr;
  uint64_t size = 0;

  Buffer() = default;

  Buffer(uint64_t size) { Allocate(size); }

  Buffer(const void* data, uint64_t size) : data((uint8_t*)data), size(size) {}

  Buffer(const Buffer&) = default;

  static Buffer Copy(Buffer other) {
    Buffer result(other.size);
    memcpy(result.data, other.data, other.size);
    return result;
  }

  void Allocate(uint64_t size) {
    Release();

    data = (uint8_t*)malloc(size);
    this->size = size;
  }

  void Release() {
    free(data);
    data = nullptr;
    size = 0;
  }

  template <typename T>
  T* As() {
    return (T*)data;
  }

  operator bool() const { return (bool)data; }
};

struct ScopedBuffer {
  ScopedBuffer(Buffer buffer) : buffer_(buffer) {}

  ScopedBuffer(uint64_t size) : buffer_(size) {}

  ~ScopedBuffer() { buffer_.Release(); }

  uint8_t* Data() { return buffer_.data; }
  uint64_t Size() { return buffer_.size; }

  template <typename T>
  T* As() {
    return buffer_.As<T>();
  }

  operator bool() const { return buffer_; }

 private:
  Buffer buffer_;
};

}  // namespace eve
