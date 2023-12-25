// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/engine_api.h"

#include <sol/overload.hpp>
#include <sol/sol.hpp>
#include <sol/types.hpp>

#include "core/event/input.h"
#include "core/event/key_code.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "scene/transform.h"

static void RegisterVectorTypes(sol::state* lua);

static void RegisterMathFunctions(sol::state* lua);

static void RegisterInputFunctions(sol::state* lua);

static void RegisterKeyCodes(sol::state* lua);

static void RegisterMouseCodes(sol::state* lua);

void RegisterTypes(sol::state* lua) {
  RegisterVectorTypes(lua);
  RegisterMathFunctions(lua);

  RegisterKeyCodes(lua);
  RegisterMouseCodes(lua);
  RegisterInputFunctions(lua);
}

template <typename T>
auto GetMultiplyOperatorOverload() {
  return sol::overload(
      [](const T& lhs, const T& rhs) -> T { return lhs * rhs; },
      [](const T& lhs, float rhs) -> T { return lhs * rhs; },
      [](float lhs, const T& rhs) -> T { return lhs * rhs; });
}

template <typename T>
auto GetDivisionOperatorOverload() {
  return sol::overload(
      [](const T& lhs, const T& rhs) -> T { return lhs / rhs; },
      [](const T& lhs, float rhs) -> T { return lhs / rhs; },
      [](float lhs, const T& rhs) -> T { return lhs / rhs; });
}

template <typename T>
auto GetAdditionOperatorOverload() {
  return sol::overload(
      [](const T& lhs, const T& rhs) -> T { return lhs + rhs; },
      [](const T& lhs, float rhs) -> T { return lhs + rhs; },
      [](float lhs, const T& rhs) -> T { return lhs + rhs; });
}

template <typename T>
auto GetSubstractionOperatorOverload() {
  return sol::overload(
      [](const T& lhs, const T& rhs) -> T { return lhs - rhs; },
      [](const T& lhs, float rhs) -> T { return lhs - rhs; },
      [](float lhs, const T& rhs) -> T { return lhs - rhs; });
}

void RegisterVectorTypes(sol::state* lua) {
  lua->new_usertype<glm::vec2>(
      "Vec2",                                                     //
      sol::constructors<glm::vec2(), glm::vec2(float, float)>(),  //
      "x", &glm::vec2::x,                                         //
      "y", &glm::vec2::y,                                         //
      sol::meta_function::multiplication,
      GetMultiplyOperatorOverload<glm::vec2>(), sol::meta_function::division,
      GetDivisionOperatorOverload<glm::vec2>(), sol::meta_function::addition,
      GetAdditionOperatorOverload<glm::vec2>(), sol::meta_function::subtraction,
      GetSubstractionOperatorOverload<glm::vec2>());

  lua->new_usertype<glm::vec3>(
      "Vec3",                                                            //
      sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),  //
      "x", &glm::vec3::x,                                                //
      "y", &glm::vec3::y,                                                //
      "z", &glm::vec3::z,                                                //
      sol::meta_function::multiplication,
      GetMultiplyOperatorOverload<glm::vec3>(), sol::meta_function::division,
      GetDivisionOperatorOverload<glm::vec3>(), sol::meta_function::addition,
      GetAdditionOperatorOverload<glm::vec3>(), sol::meta_function::subtraction,
      GetSubstractionOperatorOverload<glm::vec3>());

  lua->new_usertype<glm::vec4>(
      "Vec4",  //
      sol::constructors<glm::vec4(),
                        glm::vec4(float, float, float, float)>(),  //
      "x", &glm::vec4::x,                                          //
      "y", &glm::vec4::y,                                          //
      "z", &glm::vec4::z,                                          //
      sol::meta_function::multiplication,
      GetMultiplyOperatorOverload<glm::vec4>(), sol::meta_function::division,
      GetDivisionOperatorOverload<glm::vec4>(), sol::meta_function::addition,
      GetAdditionOperatorOverload<glm::vec4>(), sol::meta_function::subtraction,
      GetSubstractionOperatorOverload<glm::vec4>());
}

static void RegisterMathFunctions(sol::state* lua) {
  // Register GLM functions
  auto glm_funcs = lua->create_table();
  {
    glm_funcs.set_function(
        "Length",
        sol::overload(sol::resolve<float(const glm::vec2&)>(glm::length),
                      sol::resolve<float(const glm::vec3&)>(glm::length),
                      sol::resolve<float(const glm::vec4&)>(glm::length)));
    glm_funcs.set_function(
        "Distance",
        sol::overload(sol::resolve<float(const glm::vec2&, const glm::vec2&)>(
                          glm::distance),
                      sol::resolve<float(const glm::vec3&, const glm::vec3&)>(
                          glm::distance),
                      sol::resolve<float(const glm::vec4&, const glm::vec4&)>(
                          glm::distance)));
    glm_funcs.set_function(
        "Normalize",
        sol::overload(
            sol::resolve<glm::vec2(const glm::vec2&)>(glm::normalize),
            sol::resolve<glm::vec3(const glm::vec3&)>(glm::normalize),
            sol::resolve<glm::vec4(const glm::vec4&)>(glm::normalize)));
    glm_funcs.set_function(
        "Cross",
        sol::overload(
            sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(
                glm::cross),
            sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(
                glm::cross),
            sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(
                glm::cross)));
    glm_funcs.set_function(
        "Dot",
        sol::overload(
            sol::resolve<float(const glm::vec2&, const glm::vec2&)>(glm::dot),
            sol::resolve<float(const glm::vec3&, const glm::vec3&)>(glm::dot),
            sol::resolve<float(const glm::vec4&, const glm::vec4&)>(glm::dot)));
  }
  lua->globals()["Mathf"] = glm_funcs;

  lua->new_usertype<Transform>(
      "Transform",  //
      sol::constructors<Transform(),
                        Transform(const glm::vec3&, const glm::vec3&,
                                  const glm::vec3&)>(),  //
      "position", &Transform::position,                  //
      "rotation", &Transform::rotation,                  //
      "scale", &Transform::scale,                        //
      "Translate", &Transform::Translate,                //
      "Rotate", &Transform::Rotate,                      //
      "GetForward", &Transform::GetForward,              //
      "GetRight", &Transform::GetRight,                  //
      "GetUp", &Transform::GetUp,                        //
      "GetDirection", &Transform::GetDirection           //
  );
}

void RegisterInputFunctions(sol::state* lua) {
  lua->set_function("IsKeyPressed", &Input::IsKeyPressed);
  lua->set_function("IsKeyReleased", &Input::IsKeyReleased);
  lua->set_function("IsMouseButtonPressed", &Input::IsMouseButtonPressed);
  lua->set_function("IsMouseButtonReleased", &Input::IsMouseButtonReleased);
  lua->set_function("GetMousePosition", &Input::GetMousePosition);
}

void RegisterKeyCodes(sol::state* lua) {
  (*lua)["KeyCode"] = lua->create_table_with(
      "Space", KeyCode::kSpace, "Num0", KeyCode::k0, "Num1", KeyCode::k1,
      "Num2", KeyCode::k2, "Num3", KeyCode::k3, "Num4", KeyCode::k4, "Num5",
      KeyCode::k5, "Num6", KeyCode::k6, "Num7", KeyCode::k7, "Num8",
      KeyCode::k8, "Num9", KeyCode::k9, "A", KeyCode::kA, "B", KeyCode::kB, "C",
      KeyCode::kC, "D", KeyCode::kD, "E", KeyCode::kE, "F", KeyCode::kF, "G",
      KeyCode::kG, "H", KeyCode::kH, "I", KeyCode::kI, "J", KeyCode::kJ, "K",
      KeyCode::kK, "L", KeyCode::kL, "M", KeyCode::kM, "N", KeyCode::kN, "O",
      KeyCode::kO, "P", KeyCode::kP, "Q", KeyCode::kQ, "R", KeyCode::kR, "S",
      KeyCode::kS, "T", KeyCode::kT, "U", KeyCode::kU, "V", KeyCode::kV, "W",
      KeyCode::kW, "X", KeyCode::kX, "Y", KeyCode::kY, "Z", KeyCode::kZ,
      KeyCode::kWorld2, "Escape", KeyCode::kEscape, "Enter", KeyCode::kEnter,
      "Tab", KeyCode::kTab, "Backspace", KeyCode::kBackspace, "Delete",
      KeyCode::kDelete, "Right", KeyCode::kRight, "Left", KeyCode::kLeft,
      "Down", KeyCode::kDown, "Up", KeyCode::kUp, "CapsLock",
      KeyCode::kCapsLock, "F1", KeyCode::kF1, "F2", KeyCode::kF2, "F3",
      KeyCode::kF3, "F4", KeyCode::kF4, "F5", KeyCode::kF5, "F6", KeyCode::kF6,
      "F7", KeyCode::kF7, "F8", KeyCode::kF8, "F9", KeyCode::kF9, "F10",
      KeyCode::kF10, "F11", KeyCode::kF11, "F12", "Kp0", KeyCode::kKp0, "Kp1",
      KeyCode::kKp1, "Kp2", KeyCode::kKp2, "Kp3", KeyCode::kKp3, "Kp4",
      KeyCode::kKp4, "Kp5", KeyCode::kKp5, "Kp6", KeyCode::kKp6, "Kp7",
      KeyCode::kKp7, "Kp8", KeyCode::kKp8, "Kp9", KeyCode::kKp9, "LeftShift",
      KeyCode::kLeftShift, "LeftControl", KeyCode::kLeftControl, "LeftAlt",
      KeyCode::kLeftAlt, "LeftSuper", KeyCode::kLeftSuper, "RightShift",
      KeyCode::kRightShift, "RightControl", KeyCode::kRightControl, "RightAlt",
      KeyCode::kRightAlt);
}

void RegisterMouseCodes(sol::state* lua) {
  (*lua)["MouseCode"] = lua->create_table_with(
      "Button1", MouseCode::k1, "Button2", MouseCode::k2, "Button3",
      MouseCode::k3, "Button4", MouseCode::k4, "Button5", MouseCode::k5,
      "Button6", MouseCode::k6, "Button7", MouseCode::k7, "Button8",
      MouseCode::k8, "Left", MouseCode::kLeft, "Right", MouseCode::kRight,
      "Middle", MouseCode::kMiddle);
}
