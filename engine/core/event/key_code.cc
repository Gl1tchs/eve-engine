// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "core/event/key_code.h"

namespace eve {

std::string GetKeyCodeString(const KeyCode& key_code) {
  switch (key_code) {
    case KeyCode::kNone:
      return "None";
    case KeyCode::kSpace:
      return "Space";
    case KeyCode::kApostrophe:
      return "Apostrophe";
    case KeyCode::kComma:
      return "Comma";
    case KeyCode::kMinus:
      return "Minus";
    case KeyCode::kPeriod:
      return "Period";
    case KeyCode::kSlash:
      return "Slash";
    case KeyCode::k0:
      return "0";
    case KeyCode::k1:
      return "1";
    case KeyCode::k2:
      return "2";
    case KeyCode::k3:
      return "3";
    case KeyCode::k4:
      return "4";
    case KeyCode::k5:
      return "5";
    case KeyCode::k6:
      return "6";
    case KeyCode::k7:
      return "7";
    case KeyCode::k8:
      return "8";
    case KeyCode::k9:
      return "9";
    case KeyCode::kSemicolon:
      return "Semicolon";
    case KeyCode::kEqual:
      return "Equal";
    case KeyCode::kA:
      return "A";
    case KeyCode::kB:
      return "B";
    case KeyCode::kC:
      return "C";
    case KeyCode::kD:
      return "D";
    case KeyCode::kE:
      return "E";
    case KeyCode::kF:
      return "F";
    case KeyCode::kG:
      return "G";
    case KeyCode::kH:
      return "H";
    case KeyCode::kI:
      return "I";
    case KeyCode::kJ:
      return "J";
    case KeyCode::kK:
      return "K";
    case KeyCode::kL:
      return "L";
    case KeyCode::kM:
      return "M";
    case KeyCode::kN:
      return "N";
    case KeyCode::kO:
      return "O";
    case KeyCode::kP:
      return "P";
    case KeyCode::kQ:
      return "Q";
    case KeyCode::kR:
      return "R";
    case KeyCode::kS:
      return "S";
    case KeyCode::kT:
      return "T";
    case KeyCode::kU:
      return "U";
    case KeyCode::kV:
      return "V";
    case KeyCode::kW:
      return "W";
    case KeyCode::kX:
      return "X";
    case KeyCode::kY:
      return "Y";
    case KeyCode::kZ:
      return "Z";
    case KeyCode::kLeftBracket:
      return "LeftBracket";
    case KeyCode::kBackslash:
      return "Backslash";
    case KeyCode::kRightBracket:
      return "RightBracket";
    case KeyCode::kGraveAccent:
      return "GraveAccent";
    case KeyCode::kWorld1:
      return "World1";
    case KeyCode::kWorld2:
      return "World2";
    case KeyCode::kEscape:
      return "Escape";
    case KeyCode::kEnter:
      return "Enter";
    case KeyCode::kTab:
      return "Tab";
    case KeyCode::kBackspace:
      return "Backspace";
    case KeyCode::kInsert:
      return "Insert";
    case KeyCode::kDelete:
      return "Delete";
    case KeyCode::kRight:
      return "Right";
    case KeyCode::kLeft:
      return "Left";
    case KeyCode::kDown:
      return "Down";
    case KeyCode::kUp:
      return "Up";
    case KeyCode::kPageUp:
      return "PageUp";
    case KeyCode::kPageDown:
      return "PageDown";
    case KeyCode::kHome:
      return "Home";
    case KeyCode::kEnd:
      return "End";
    case KeyCode::kCapsLock:
      return "CapsLock";
    case KeyCode::kScrollLock:
      return "ScrollLock";
    case KeyCode::kNumLock:
      return "NumLock";
    case KeyCode::kPrintScreen:
      return "PrintScreen";
    case KeyCode::kPause:
      return "Pause";
    case KeyCode::kF1:
      return "F1";
    case KeyCode::kF2:
      return "F2";
    case KeyCode::kF3:
      return "F3";
    case KeyCode::kF4:
      return "F4";
    case KeyCode::kF5:
      return "F5";
    case KeyCode::kF6:
      return "F6";
    case KeyCode::kF7:
      return "F7";
    case KeyCode::kF8:
      return "F8";
    case KeyCode::kF9:
      return "F9";
    case KeyCode::kF10:
      return "F10";
    case KeyCode::kF11:
      return "F11";
    case KeyCode::kF12:
      return "F12";
    case KeyCode::kF13:
      return "F13";
    case KeyCode::kF14:
      return "F14";
    case KeyCode::kF15:
      return "F15";
    case KeyCode::kF16:
      return "F16";
    case KeyCode::kF17:
      return "F17";
    case KeyCode::kF18:
      return "F18";
    case KeyCode::kF19:
      return "F19";
    case KeyCode::kF20:
      return "F20";
    case KeyCode::kF21:
      return "F21";
    case KeyCode::kF22:
      return "F22";
    case KeyCode::kF23:
      return "F23";
    case KeyCode::kF24:
      return "F24";
    case KeyCode::kF25:
      return "F25";
    case KeyCode::kKp0:
      return "Kp0";
    case KeyCode::kKp1:
      return "Kp1";
    case KeyCode::kKp2:
      return "Kp2";
    case KeyCode::kKp3:
      return "Kp3";
    case KeyCode::kKp4:
      return "Kp4";
    case KeyCode::kKp5:
      return "Kp5";
    case KeyCode::kKp6:
      return "Kp6";
    case KeyCode::kKp7:
      return "Kp7";
    case KeyCode::kKp8:
      return "Kp8";
    case KeyCode::kKp9:
      return "Kp9";
    case KeyCode::kKpDecimal:
      return "KpDecimal";
    case KeyCode::kKpDivide:
      return "KpDivide";
    case KeyCode::kKpMultiply:
      return "KpMultiply";
    case KeyCode::kKpSubtract:
      return "KpSubtract";
    case KeyCode::kKpAdd:
      return "KpAdd";
    case KeyCode::kKpEnter:
      return "KpEnter";
    case KeyCode::kKpEqual:
      return "KpEqual";
    case KeyCode::kLeftShift:
      return "LeftShift";
    case KeyCode::kLeftControl:
      return "LeftControl";
    case KeyCode::kLeftAlt:
      return "LeftAlt";
    case KeyCode::kLeftSuper:
      return "LeftSuper";
    case KeyCode::kRightShift:
      return "RightShift";
    case KeyCode::kRightControl:
      return "RightControl";
    case KeyCode::kRightAlt:
      return "RightAlt";
    case KeyCode::kRightSuper:
      return "RightSuper";
    case KeyCode::kMenu:
      return "Menu";
    default:
      return "None";
  }
}

KeyCode GetKeyCode(const std::string& key_code) {
  if (key_code == "Space") {
    return KeyCode::kSpace;
  } else if (key_code == "Apostrophe") {
    return KeyCode::kApostrophe;
  } else if (key_code == "Comma") {
    return KeyCode::kComma;
  } else if (key_code == "Minus") {
    return KeyCode::kMinus;
  } else if (key_code == "Period") {
    return KeyCode::kPeriod;
  } else if (key_code == "Slash") {
    return KeyCode::kSlash;
  } else if (key_code == "0") {
    return KeyCode::k0;
  } else if (key_code == "1") {
    return KeyCode::k1;
  } else if (key_code == "2") {
    return KeyCode::k2;
  } else if (key_code == "3") {
    return KeyCode::k3;
  } else if (key_code == "4") {
    return KeyCode::k4;
  } else if (key_code == "5") {
    return KeyCode::k5;
  } else if (key_code == "6") {
    return KeyCode::k6;
  } else if (key_code == "7") {
    return KeyCode::k7;
  } else if (key_code == "8") {
    return KeyCode::k8;
  } else if (key_code == "9") {
    return KeyCode::k9;
  } else if (key_code == "Semicolon") {
    return KeyCode::kSemicolon;
  } else if (key_code == "Equal") {
    return KeyCode::kEqual;
  } else if (key_code == "A") {
    return KeyCode::kA;
  } else if (key_code == "B") {
    return KeyCode::kB;
  } else if (key_code == "C") {
    return KeyCode::kC;
  } else if (key_code == "D") {
    return KeyCode::kD;
  } else if (key_code == "E") {
    return KeyCode::kE;
  } else if (key_code == "F") {
    return KeyCode::kF;
  } else if (key_code == "G") {
    return KeyCode::kG;
  } else if (key_code == "H") {
    return KeyCode::kH;
  } else if (key_code == "I") {
    return KeyCode::kI;
  } else if (key_code == "J") {
    return KeyCode::kJ;
  } else if (key_code == "K") {
    return KeyCode::kK;
  } else if (key_code == "L") {
    return KeyCode::kL;
  } else if (key_code == "M") {
    return KeyCode::kM;
  } else if (key_code == "N") {
    return KeyCode::kN;
  } else if (key_code == "O") {
    return KeyCode::kO;
  } else if (key_code == "P") {
    return KeyCode::kP;
  } else if (key_code == "Q") {
    return KeyCode::kQ;
  } else if (key_code == "R") {
    return KeyCode::kR;
  } else if (key_code == "S") {
    return KeyCode::kS;
  } else if (key_code == "T") {
    return KeyCode::kT;
  } else if (key_code == "U") {
    return KeyCode::kU;
  } else if (key_code == "V") {
    return KeyCode::kV;
  } else if (key_code == "W") {
    return KeyCode::kW;
  } else if (key_code == "X") {
    return KeyCode::kX;
  } else if (key_code == "Y") {
    return KeyCode::kY;
  } else if (key_code == "Z") {
    return KeyCode::kZ;
  } else if (key_code == "LeftBracket") {
    return KeyCode::kLeftBracket;
  } else if (key_code == "Backslash") {
    return KeyCode::kBackslash;
  } else if (key_code == "RightBracket") {
    return KeyCode::kRightBracket;
  } else if (key_code == "GraveAccent") {
    return KeyCode::kGraveAccent;
  } else if (key_code == "World1") {
    return KeyCode::kWorld1;
  } else if (key_code == "World2") {
    return KeyCode::kWorld2;
  } else if (key_code == "Escape") {
    return KeyCode::kEscape;
  } else if (key_code == "Enter") {
    return KeyCode::kEnter;
  } else if (key_code == "Tab") {
    return KeyCode::kTab;
  } else if (key_code == "Backspace") {
    return KeyCode::kBackspace;
  } else if (key_code == "Insert") {
    return KeyCode::kInsert;
  } else if (key_code == "Delete") {
    return KeyCode::kDelete;
  } else if (key_code == "Right") {
    return KeyCode::kRight;
  } else if (key_code == "Left") {
    return KeyCode::kLeft;
  } else if (key_code == "Down") {
    return KeyCode::kDown;
  } else if (key_code == "Up") {
    return KeyCode::kUp;
  } else if (key_code == "PageUp") {
    return KeyCode::kPageUp;
  } else if (key_code == "PageDown") {
    return KeyCode::kPageDown;
  } else if (key_code == "Home") {
    return KeyCode::kHome;
  } else if (key_code == "End") {
    return KeyCode::kEnd;
  } else if (key_code == "CapsLock") {
    return KeyCode::kCapsLock;
  } else if (key_code == "ScrollLock") {
    return KeyCode::kScrollLock;
  } else if (key_code == "NumLock") {
    return KeyCode::kNumLock;
  } else if (key_code == "PrintScreen") {
    return KeyCode::kPrintScreen;
  } else if (key_code == "Pause") {
    return KeyCode::kPause;
  } else if (key_code == "F1") {
    return KeyCode::kF1;
  } else if (key_code == "F2") {
    return KeyCode::kF2;
  } else if (key_code == "F3") {
    return KeyCode::kF3;
  } else if (key_code == "F4") {
    return KeyCode::kF4;
  } else if (key_code == "F5") {
    return KeyCode::kF5;
  } else if (key_code == "F6") {
    return KeyCode::kF6;
  } else if (key_code == "F7") {
    return KeyCode::kF7;
  } else if (key_code == "F8") {
    return KeyCode::kF8;
  } else if (key_code == "F9") {
    return KeyCode::kF9;
  } else if (key_code == "F10") {
    return KeyCode::kF10;
  } else if (key_code == "F11") {
    return KeyCode::kF11;
  } else if (key_code == "F12") {
    return KeyCode::kF12;
  } else if (key_code == "F13") {
    return KeyCode::kF13;
  } else if (key_code == "F14") {
    return KeyCode::kF14;
  } else if (key_code == "F15") {
    return KeyCode::kF15;
  } else if (key_code == "F16") {
    return KeyCode::kF16;
  } else if (key_code == "F17") {
    return KeyCode::kF17;
  } else if (key_code == "F18") {
    return KeyCode::kF18;
  } else if (key_code == "F19") {
    return KeyCode::kF19;
  } else if (key_code == "F20") {
    return KeyCode::kF20;
  } else if (key_code == "F21") {
    return KeyCode::kF21;
  } else if (key_code == "F22") {
    return KeyCode::kF22;
  } else if (key_code == "F23") {
    return KeyCode::kF23;
  } else if (key_code == "F24") {
    return KeyCode::kF24;
  } else if (key_code == "F25") {
    return KeyCode::kF25;
  } else if (key_code == "Kp0") {
    return KeyCode::kKp0;
  } else if (key_code == "Kp1") {
    return KeyCode::kKp1;
  } else if (key_code == "Kp2") {
    return KeyCode::kKp2;
  } else if (key_code == "Kp3") {
    return KeyCode::kKp3;
  } else if (key_code == "Kp4") {
    return KeyCode::kKp4;
  } else if (key_code == "Kp5") {
    return KeyCode::kKp5;
  } else if (key_code == "Kp6") {
    return KeyCode::kKp6;
  } else if (key_code == "Kp7") {
    return KeyCode::kKp7;
  } else if (key_code == "Kp8") {
    return KeyCode::kKp8;
  } else if (key_code == "Kp9") {
    return KeyCode::kKp9;
  } else if (key_code == "KpDecimal") {
    return KeyCode::kKpDecimal;
  } else if (key_code == "KpDivide") {
    return KeyCode::kKpDivide;
  } else if (key_code == "KpMultiply") {
    return KeyCode::kKpMultiply;
  } else if (key_code == "KpSubtract") {
    return KeyCode::kKpSubtract;
  } else if (key_code == "KpAdd") {
    return KeyCode::kKpAdd;
  } else if (key_code == "KpEnter") {
    return KeyCode::kKpEnter;
  } else if (key_code == "KpEqual") {
    return KeyCode::kKpEqual;
  } else if (key_code == "LeftShift") {
    return KeyCode::kLeftShift;
  } else if (key_code == "LeftControl") {
    return KeyCode::kLeftControl;
  } else if (key_code == "LeftAlt") {
    return KeyCode::kLeftAlt;
  } else if (key_code == "LeftSuper") {
    return KeyCode::kLeftSuper;
  } else if (key_code == "RightShift") {
    return KeyCode::kRightShift;
  } else if (key_code == "RightControl") {
    return KeyCode::kRightControl;
  } else if (key_code == "RightAlt") {
    return KeyCode::kRightAlt;
  } else if (key_code == "RightSuper") {
    return KeyCode::kRightSuper;
  } else if (key_code == "Menu") {
    return KeyCode::kMenu;
  } else {
    return KeyCode::kNone;
  }
}

}  // namespace eve