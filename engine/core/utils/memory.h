// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <memory>

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args);

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args);

#include "core/utils/memory.inl"
