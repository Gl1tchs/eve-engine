// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scripting/engine_api.h"

#include <sol/sol.hpp>

#include "scene/transform.h"

void RegisterTypes(sol::state* lua) {
  lua->new_usertype<glm::vec3>(
      "Vec3",                                                            //
      sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),  //
      "x", &glm::vec3::x,                                                //
      "y", &glm::vec3::y,                                                //
      "z", &glm::vec3::z                                                 //
  );

  lua->new_usertype<Transform>(
      "Transform",  //
      sol::constructors<Transform(),
                        Transform(const glm::vec3&, const glm::vec3&,
                                  const glm::vec3&)>(),  //
      "position", &Transform::position,                  //
      "rotation", &Transform::rotation,                  //
      "scale", &Transform::scale                         //
  );
}
