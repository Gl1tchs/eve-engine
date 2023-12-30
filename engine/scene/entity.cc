// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/entity.h"

namespace eve {
Entity::Entity(entt::entity handle, Scene* scene)
    : entity_handle_(handle), scene_(scene) {}
}  // namespace eve
