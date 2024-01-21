// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene.h"

#include "physics/physics_system.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/transform.h"
#include "scripting/script.h"
#include "scripting/script_engine.h"

namespace eve {

Scene::Scene(Ref<State> state, std::string name) : state_(state), name_(name) {
  PushSystem<PhysicsSystem>();
}

Scene::~Scene() {
  for (auto system : systems_) {
    delete system;
  }
}

bool Scene::OnRuntimeStart() {
  is_running_ = true;

  auto& renderer = state_->renderer;
  if (renderer->CustomShadersProvided()) {
    renderer->ResetShaderData();
  }

  ScriptEngine::OnRuntimeStart(this);

  {
    auto view = registry_.view<ScriptComponent>();

    // entity script instance creation should be done in two iterations
    // because field type of entity depends on other entities
    // thats why we are creating the mono instance first and
    // assign not managed fields, then assigning the managed fields.

    // first iteration create mono instances
    for (auto entity_id : view) {
      Entity entity = {entity_id, this};
      ScriptEngine::CreateEntityInstance(entity);
    }

    // second iteration set field values and invoke on create
    for (auto entity_id : view) {
      Entity entity = {entity_id, this};
      ScriptEngine::SetEntityManagedFieldValues(entity);
      ScriptEngine::InvokeCreateEntity(entity);
    }
  }

  for (auto system : systems_) {
    if (!system->IsRuntime()) {
      continue;
    }

    system->OnStart();
  }

  return true;
}

void Scene::OnUpdateRuntime(float ds) {
  if (is_paused_ && step_frames_-- <= 0) {
    return;
  }

  auto view = registry_.view<ScriptComponent>();
  for (auto entity_id : view) {
    Entity entity = {entity_id, this};
    ScriptEngine::InvokeUpdateEntity(entity, ds);
  }

  for (auto system : systems_) {
    if (!system->IsRuntime()) {
      continue;
    }

    system->OnUpdate(ds);
  }
}

void Scene::OnRuntimeStop() {
  is_running_ = false;

  auto view = registry_.view<ScriptComponent>();
  for (auto entity_id : view) {
    Entity entity = {entity_id, this};
    ScriptEngine::InvokeDestroyEntity(entity);
  }

  ScriptEngine::OnRuntimeStop();

  for (auto system : systems_) {
    if (!system->IsRuntime()) {
      continue;
    }

    system->OnStop();
  }
}

void Scene::OnUpdateEditor(float ds) {
  for (auto system : systems_) {
    if (!system->IsEditor()) {
      continue;
    }

    system->OnUpdate(ds);
  }
}

void Scene::Step(int frames) {
  step_frames_ = frames;
}

Entity Scene::CreateEntity(const EntityCreateInfo& info) {
  return CreateEntityWithUUID(UUID(), info);
}

Entity Scene::CreateEntityWithUUID(UUID uuid, const EntityCreateInfo& info) {
  Entity entity = {registry_.create(), this};

  entity.AddComponent<IdComponent>(uuid);
  entity.AddComponent<Transform>();
  entity.AddComponent<RelationComponent>();

  // Set an unique tag
  auto& tag = entity.AddComponent<TagComponent>();
  tag.tag = GetEntityName(info.name);

  // Set parent if provided.
  if (info.parent_id) {
    Entity parent = TryGetEntityByUUID(info.parent_id);
    if (parent) {
      entity.SetParent(parent);
    }
  }

  entity_map_[uuid] = entity;

  return entity;
}

void Scene::DestroyEntity(Entity entity) {
  for (auto child : entity.GetChildren()) {
    DestroyEntity(child);
  }

  if (IsRunning()) {
    ScriptEngine::InvokeDestroyEntity(entity);
  }

  entity_map_.erase(entity.GetUUID());
  registry_.destroy(entity);
}

bool Scene::Exists(Entity entity) {
  if (!entity.HasComponent<IdComponent>()) {
    return false;
  }

  return entity_map_.find(entity.GetUUID()) != entity_map_.end();
}

Entity Scene::TryGetEntityByUUID(UUID uuid) {
  if (entity_map_.find(uuid) != entity_map_.end()) {
    return {entity_map_.at(uuid), this};
  }
  return {};
}

Entity Scene::TryGetEntityByName(std::string name) {
  auto view = registry_.view<TagComponent>();
  for (auto entity : view) {
    const TagComponent& tc = view.get<TagComponent>(entity);
    if (tc.tag == name)
      return {entity, this};
  }
  return {};
}

Entity Scene::GetPrimaryCameraEntity() {
  auto view = registry_.view<CameraComponent>();
  for (auto entity : view) {
    const auto& camera = view.get<CameraComponent>(entity);
    if (camera.is_primary)
      return {entity, this};
  }
  return {};
}

Entity Scene::GetSelectedEntity() {
  return selected_entity_ != nullptr ? *selected_entity_ : kInvalidEntity;
}

bool Scene::EntityNameExists(const std::string& name) {
  const auto it =
      std::find_if(entity_map_.begin(), entity_map_.end(), [&](auto& pair) {
        const auto& tag_comp =
            pair.second.template GetComponent<TagComponent>();
        return tag_comp.tag == name;
      });
  return it != entity_map_.end();
}

std::string Scene::GetEntityName(const std::string& name) {
  static int default_counter = 0;
  if (!name.empty()) {

    std::string name_unique = name;
    int counter = 1;

    while (EntityNameExists(name_unique)) {
      name_unique = std::format("{0} ({1})", name, counter);
      counter++;
    }

    return name_unique;
  } else if (name.empty() && default_counter == 0) {
    default_counter++;

    return "Entity";
  } else {
    default_counter++;

    return std::format("Entity ({0})", default_counter);
  }
}

template <typename... Component>
static void CopyComponent(
    entt::registry& dst, entt::registry& src,
    const std::unordered_map<UUID, entt::entity>& entt_map) {
  (
      [&]() {
        auto view = src.view<Component>();
        for (auto src_entity : view) {
          entt::entity dst_entity =
              entt_map.at(src.get<IdComponent>(src_entity).id);

          auto& src_component = src.get<Component>(src_entity);
          dst.emplace_or_replace<Component>(dst_entity, src_component);
        }
      }(),
      ...);
}

template <typename... Component>
static void CopyComponent(
    ComponentGroup<Component...>, entt::registry& dst, entt::registry& src,
    const std::unordered_map<UUID, entt::entity>& entt_map) {
  CopyComponent<Component...>(dst, src, entt_map);
}

template <typename... Component>
static void CopyComponentIfExists(Entity dst, Entity src) {
  (
      [&]() {
        if (src.HasComponent<Component>())
          dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
      }(),
      ...);
}

template <typename... Component>
static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst,
                                  Entity src) {
  CopyComponentIfExists<Component...>(dst, src);
}

Ref<Scene> Scene::Copy(Ref<Scene> src) {
  Ref<Scene> dst_scene = CreateRef<Scene>(src->state_, src->name_);

  auto& src_scene_registry = src->registry_;
  auto& dst_scene_registry = dst_scene->registry_;
  std::unordered_map<UUID, entt::entity> entt_map;

  // Create entities in new scene
  auto id_view = src_scene_registry.view<IdComponent>();
  for (auto entity_id : id_view) {
    UUID uuid = src_scene_registry.get<IdComponent>(entity_id).id;
    const auto& name = src_scene_registry.get<TagComponent>(entity_id).tag;

    Entity new_entity =
        dst_scene->CreateEntityWithUUID(uuid, {name, kInvalidUUID});

    // set parent id but do not set children vectors
    const auto& relation = src_scene_registry.get<RelationComponent>(entity_id);
    new_entity.GetRelation().parent_id = relation.parent_id;

    entt_map[uuid] = (entt::entity)new_entity;
  }

  CopyComponent(AllComponents{}, dst_scene_registry, src_scene_registry,
                entt_map);

  // set child/parent relations
  for (auto& [uuid, entity] : dst_scene->entity_map_) {
    // Set parent entity
    auto& relation = entity.GetRelation();
    if (relation.parent_id) {
      auto parent_entity = dst_scene->TryGetEntityByUUID(relation.parent_id);
      if (parent_entity) {
        entity.SetParent(parent_entity);
      }
    }
  }

  return dst_scene;
}

}  // namespace eve
