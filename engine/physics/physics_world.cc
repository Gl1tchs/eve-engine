// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "physics/physics_world.h"

#include "physics/box_collider.h"
#include "physics/rigidbody.h"
#include "scene/entity.h"
#include "scene/scene.h"

namespace eve {

Scene* PhysicsWorld::scene_ = nullptr;
PhysicsWorldSettings PhysicsWorld::settings_;

void ApplyConstraints(glm::vec3& vec, const glm::vec3& vec_before,
                      glm::bvec3 constraints) {
  for (int i = 0; i < 3; ++i) {
    if (constraints[i]) {
      vec[i] = vec_before[i];
    }
  }
}

void PhysicsWorld::OnStart(Scene* scene) {
  scene_ = scene;

  if (!scene_) {
    return;
  }
}

void PhysicsWorld::OnUpdate(float ds) {
  if (!scene_) {
    return;
  }

  // TODO If this performs not good with big data consider using Barnes-Hut algorithm
  // with an Octree
  for (const auto& entity_id :
       scene_->GetAllEntitiesWith<Transform, Rigidbody>()) {
    Entity entity{entity_id, scene_};

    Transform& tc = entity.GetComponent<Transform>();
    Rigidbody& rb = entity.GetComponent<Rigidbody>();

    Transform tc_before = tc;
    Rigidbody rb_before = rb;

    tc.position += rb.velocity * ds + 0.5f * rb.acceleration * ds * ds;

    rb.velocity += rb.acceleration * ds;
    if (rb.use_gravity) {
      rb.velocity += settings_.gravity * ds;
    }

    // Apply constraints
    glm::bvec3 position_constraints;
    std::memcpy(&position_constraints, &rb.position_constraints,
                sizeof(glm::bvec3));

    ApplyConstraints(tc.position, tc_before.position, position_constraints);
    ApplyConstraints(rb.velocity, rb_before.velocity, position_constraints);

    glm::bvec3 rotation_constraints;
    std::memcpy(&rotation_constraints, &rb.rotation_constraints,
                sizeof(glm::bvec3));

    ApplyConstraints(tc.rotation, tc_before.rotation, rotation_constraints);

    if (!entity.HasComponent<BoxCollider>()) {
      continue;
    }

    BoxCollider& collider = entity.GetComponent<BoxCollider>();

    // Check for collisions with other entities having BoxCollider components
    scene_->GetAllEntitiesWith<Transform, BoxCollider>().each(
        [&](entt::entity other_id, Transform& other_tc,
            BoxCollider& other_collider) {
          Entity other_entity{other_id, scene_};

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

void PhysicsWorld::OnStop() {
  if (!scene_) {
    return;
  }
}

}  // namespace eve