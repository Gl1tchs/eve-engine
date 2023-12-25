// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "scene/scene.h"

#include "scene/components.h"
#include "scene/entity.h"
#include "scene/transform.h"
#include "scripting/script.h"
#include "scripting/script_engine.h"

Scene::Scene(Ref<State>& state, std::string name)
    : state_(state), name_(name) {}

template <typename... Component>
static void CopyComponent(
    entt::registry& dst, entt::registry& src,
    const std::unordered_map<GUUID, entt::entity>& entt_map) {
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
    const std::unordered_map<GUUID, entt::entity>& entt_map) {
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
  std::unordered_map<GUUID, entt::entity> entt_map;

  // Create entities in new scene
  auto id_view = src_scene_registry.view<IdComponent>();
  for (auto e : id_view) {
    GUUID uuid = src_scene_registry.get<IdComponent>(e).id;
    const auto& name = src_scene_registry.get<TagComponent>(e).tag;
    Entity new_entity = dst_scene->CreateEntityWithUUID(uuid, name);
    entt_map[uuid] = (entt::entity)new_entity;
  }

  // Copy components (except IdComponent and TagComponent)
  CopyComponent(AllComponents{}, dst_scene_registry, src_scene_registry,
                entt_map);

  return dst_scene;
}

void Scene::OnRuntimeStart() {
  is_running_ = true;

  GetAllEntitiesWith<ScriptComponent>().each(
      [this](entt::entity entity_id, ScriptComponent& sc) {
        Entity entity{entity_id, this};

        Ref<Script>& script = sc.instance;
        if (script) {
          script->entity_ = &entity;
          script->OnStart();
        }
      });
}

void Scene::OnRuntimeStop() {
  is_running_ = false;

  GetAllEntitiesWith<ScriptComponent>().each(
      [this](entt::entity entity_id, ScriptComponent& sc) {
        Entity entity{entity_id, this};

        Ref<Script>& script = sc.instance;
        if (script) {
          script->entity_ = &entity;
          script->OnDestroy();
        }
      });
}

void Scene::OnUpdateRuntime(float ds) {
  if (is_paused_ && step_frames_-- <= 0) {
    return;
  }

  GetAllEntitiesWith<ScriptComponent>().each(
      [this, &ds](entt::entity entity_id, ScriptComponent& sc) {
        // TODO maybe do not copy every frame
        Entity entity{entity_id, this};

        Ref<Script>& script = sc.instance;
        if (script) {
          script->entity_ = &entity;
          script->OnUpdate(ds);
        }
      });
}

void Scene::Step(int frames) {
  step_frames_ = frames;
}

Entity Scene::CreateEntity(const std::string& name) {
  return CreateEntityWithUUID(GUUID(), name);
}

Entity Scene::CreateEntityWithUUID(GUUID uuid, const std::string& name) {
  Entity entity = {registry_.create(), this};
  entity.AddComponent<IdComponent>(uuid);
  entity.AddComponent<Transform>();
  auto& tag = entity.AddComponent<TagComponent>();

  static int default_counter = 0;
  if (!name.empty()) {

    std::string name_unique = name;
    int counter = 1;

    while (EntityNameExists(name_unique)) {
      name_unique = std::format("{0} ({1})", name, counter);
      counter++;
    }

    tag.tag = name_unique;

  } else if (name.empty() && default_counter == 0) {
    tag.tag = "Entity";
    default_counter++;
  } else {
    tag.tag = std::format("Entity ({0})", default_counter);
    default_counter++;
  }

  entity_map_[uuid] = entity;

  return entity;
}

bool Scene::Exists(Entity entity) {
  if (!entity.HasComponent<IdComponent>()) {
    return false;
  }

  return entity_map_.find(entity.GetUUID()) != entity_map_.end();
}

void Scene::DestroyEntity(Entity entity) {
  entity_map_.erase(entity.GetUUID());
  registry_.destroy(entity);
}

std::optional<Entity> Scene::FindEntityByUUID(GUUID uuid) {
  if (entity_map_.find(uuid) != entity_map_.end()) {
    return std::optional<Entity>(Entity{entity_map_.at(uuid), this});
  }
  return std::optional<Entity>();
}

std::optional<Entity> Scene::FindEntityByName(std::string_view name) {
  auto view = registry_.view<TagComponent>();
  for (auto entity : view) {
    const TagComponent& tc = view.get<TagComponent>(entity);
    if (tc.tag == name)
      return std::optional<Entity>(Entity{entity, this});
  }
  return std::optional<Entity>();
}

std::optional<Entity> Scene::GetPrimaryCameraEntity() {
  auto view = registry_.view<CameraComponent>();
  for (auto entity : view) {
    const auto& camera = view.get<CameraComponent>(entity);
    if (camera.is_primary)
      return std::optional<Entity>(Entity{entity, this});
  }
  return std::optional<Entity>();
}

Entity Scene::GetSelectedEntity() {
  return selected_entity_ != nullptr ? *selected_entity_ : Entity{};
}

bool Scene::EntityNameExists(const std::string& name) {
  auto it =
      std::find_if(entity_map_.begin(), entity_map_.end(), [&](auto& pair) {
        const auto& tag_comp =
            pair.second.template GetComponent<TagComponent>();
        return tag_comp.tag == name;
      });

  return it != entity_map_.end();
}