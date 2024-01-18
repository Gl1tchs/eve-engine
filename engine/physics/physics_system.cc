// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "physics/physics_system.h"

#include "physics/box_collider.h"
#include "physics/rigidbody.h"
#include "scene/entity.h"
#include "scene/scene.h"

namespace eve {

void ApplyConstraints(glm::vec3& vec, const glm::vec3& vec_before,
                      glm::bvec3 constraints) {
  for (int i = 0; i < 3; ++i) {
    if (constraints[i]) {
      vec[i] = vec_before[i];
    }
  }
}

PhysicsSystem::PhysicsSystem()
    : System(SystemRunType_kRuntime | SystemRunType_kSimulation) {}

void PhysicsSystem::OnUpdate(float ds) {
  // TODO If this performs not good with big data consider using Barnes-Hut algorithm
  // with an Octree
  for (const auto& entity_id :
       GetScene()->GetAllEntitiesWith<Transform, Rigidbody>()) {
    Entity entity{entity_id, GetScene()};

    Transform& tc = entity.GetComponent<Transform>();
    Rigidbody& rb = entity.GetComponent<Rigidbody>();

    Transform tc_before = tc;
    Rigidbody rb_before = rb;

    tc.local_position += rb.velocity * ds + 0.5f * rb.acceleration * ds * ds;

    rb.velocity += rb.acceleration * ds;
    if (rb.use_gravity) {
      rb.velocity += settings_.gravity * ds;
    }

    // Apply constraints
    glm::bvec3 position_constraints;
    std::memcpy(&position_constraints, &rb.position_constraints,
                sizeof(glm::bvec3));

    ApplyConstraints(tc.local_position, tc_before.local_position,
                     position_constraints);
    ApplyConstraints(rb.velocity, rb_before.velocity, position_constraints);

    glm::bvec3 rotation_constraints;
    std::memcpy(&rotation_constraints, &rb.rotation_constraints,
                sizeof(glm::bvec3));

    ApplyConstraints(tc.local_rotation, tc_before.local_rotation,
                     rotation_constraints);

    if (!entity.HasComponent<BoxCollider>()) {
      continue;
    }

    BoxCollider& collider = entity.GetComponent<BoxCollider>();

    // Check for collisions with other entities having BoxCollider components
    GetScene()->GetAllEntitiesWith<Transform, BoxCollider>().each(
        [&](entt::entity other_id, Transform& other_tc,
            BoxCollider& other_collider) {
          Entity other_entity{other_id, GetScene()};

          if (entity_id == other_id) {
            return;
          }

          if (ColliderIntersects(tc, collider, other_tc, other_collider)) {
            rb = rb_before;
            tc = tc_before;

            if (collider.is_trigger && collider.on_trigger != nullptr) {
              collider.on_trigger(other_entity.GetName());
            }
            if (other_collider.is_trigger &&
                other_collider.on_trigger != nullptr) {
              other_collider.on_trigger(entity.GetName());
            }
          }
        });
  }
}

}  // namespace eve