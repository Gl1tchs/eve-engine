// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/event/mouse_code.h"

namespace eve {

std::string GetMouseCodeString(const MouseCode& mouse_code) {
  switch (mouse_code) {
    case MouseCode::k1:
      return "1";
    case MouseCode::k2:
      return "2";
    case MouseCode::k3:
      return "3";
    case MouseCode::k4:
      return "4";
    case MouseCode::k5:
      return "5";
    case MouseCode::k6:
      return "6";
    case MouseCode::k7:
      return "7";
    case MouseCode::k8:
      return "8";
    case MouseCode::kNone:
    default:
      return "None";
  }
}

MouseCode GetMouseCode(const std::string& mouse_code) {
  if (mouse_code == "1") {
    return MouseCode::k1;
  } else if (mouse_code == "2") {
    return MouseCode::k2;
  } else if (mouse_code == "3") {
    return MouseCode::k3;
  } else if (mouse_code == "4") {
    return MouseCode::k4;
  } else if (mouse_code == "5") {
    return MouseCode::k5;
  } else if (mouse_code == "6") {
    return MouseCode::k6;
  } else if (mouse_code == "7") {
    return MouseCode::k7;
  } else if (mouse_code == "8") {
    return MouseCode::k8;
  } else {
    return MouseCode::kNone;
  }
}

}  // namespace eve