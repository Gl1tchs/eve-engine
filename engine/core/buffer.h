// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

struct Buffer {
  uint8_t* data = nullptr;
  uint64_t size = 0;

  Buffer() = default;

  Buffer(uint64_t size);

  Buffer(const void* data, uint64_t size);

  Buffer(const Buffer&) = default;

  static Buffer Copy(Buffer other);

  void Allocate(uint64_t size);

  void Release();

  template <typename T>
  T* As() {
    return (T*)data;
  }

  operator bool() const { return (bool)data; }
};

struct ScopedBuffer {
  ScopedBuffer(Buffer buffer);
  ScopedBuffer(uint64_t size);

  ~ScopedBuffer();

  [[nodiscard]] uint8_t* GetData();
  [[nodiscard]] const uint64_t& GetSize() const;

  template <typename T>
  T* As() {
    return buffer_.As<T>();
  }

  operator bool() const { return buffer_; }

 private:
  Buffer buffer_;
};

template <typename T>
struct BufferArray {
  BufferArray() = default;

  BufferArray(const uint64_t max_elements)
      : buffer_(max_elements * sizeof(T)), max_elements_(max_elements) {}

  ~BufferArray() {
    if (buffer_) {
      buffer_.Release();
    }
  }

  [[nodiscard]] uint8_t* GetData() { return buffer_.data; }

  [[nodiscard]] const uint64_t& GetSize() const { return buffer_.size; }

  [[nodiscard]] const uint32_t& GetCount() const { return count_; }

  void Allocate(const uint64_t max_elements) {
    max_elements_ = max_elements;
    buffer_.Allocate(max_elements_ * sizeof(T));
  }

  void Release() {
    count_ = 0;
    max_elements_ = 0;
    buffer_.Release();
  }

  void Add(const T& value) {
    // prevent memory leaks
    EVE_ASSERT_ENGINE(buffer_ && count_ < max_elements_);
    buffer_.As<T>()[count_++] = value;
  }

  [[nodiscard]] T& Get(const uint32_t idx) {
    EVE_ASSERT_ENGINE(buffer_ && idx < max_elements_);
    return buffer_.As<T>()[idx];
  }

  void Clear() {
    EVE_ASSERT_ENGINE(buffer_);
    int64_t max_elements_copy = max_elements_;
    Release();
    Allocate(max_elements_copy);
  }

  void ResetIndex() { count_ = 0; }

  T& operator[](const uint32_t idx) { return Get(idx); }

  operator bool() const { return buffer_; }

 private:
  Buffer buffer_;
  uint32_t count_ = 0;

  uint32_t max_elements_ = 0;
};

}  // namespace eve
