// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <functional>
#include <string>
#include <vector>

struct Combo {
  static void Draw(const char* name, std::vector<std::string> elements,
                   std::function<void(const std::string&)> callback);
};
