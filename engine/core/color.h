// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include "pch_shared.h"

namespace eve {

struct Color {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 1.0f;

  constexpr Color() = default;

  constexpr Color(float value) : r(value), g(value), b(value), a(1.0f) {}

  constexpr Color(float red, float green, float blue, float alpha)
      : r(red), g(green), b(blue), a(alpha) {}

  constexpr Color(const glm::vec3& v3, float alpha)
      : r(v3.r), g(v3.g), b(v3.b), a(alpha) {}

  constexpr Color(const glm::vec4& v4) : r(v4.r), g(v4.g), b(v4.b), a(v4.a) {}
};

constexpr Color kColorBlack(0.0f, 0.0f, 0.0f, 1.0f);
constexpr Color kColorWhite(1.0f, 1.0f, 1.0f, 1.0f);
constexpr Color kColorRed(1.0f, 0.0f, 0.0f, 1.0f);
constexpr Color kColorGreen(0.0f, 1.0f, 0.0f, 1.0f);
constexpr Color kColorBlue(0.0f, 0.0f, 1.0f, 1.0f);
constexpr Color kColorYellow(1.0f, 1.0f, 0.0f, 1.0f);
constexpr Color kColorCyan(0.0f, 1.0f, 1.0f, 1.0f);
constexpr Color kColorMagenta(1.0f, 0.0f, 1.0f, 1.0f);
constexpr Color kColorGray(0.5f, 0.5f, 0.5f, 1.0f);
constexpr Color kColorOrange(1.0f, 0.5f, 0.0f, 1.0f);

}  // namespace eve