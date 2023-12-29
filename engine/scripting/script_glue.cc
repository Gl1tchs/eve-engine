// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "pch_shared.h"

#include "scripting/script_glue.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include "core/event/input.h"
#include "core/event/key_code.h"
#include "core/utils/guuid.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/transform.h"
#include "scripting/script_engine.h"

std::string MonoStringToString(MonoString* string) {
  char* cStr = mono_string_to_utf8(string);
  std::string str(cStr);
  mono_free(cStr);
  return str;
}

static std::unordered_map<MonoType*, std::function<bool(Entity)>>
    entity_has_component_funcs;

#define ADD_INTERNAL_CALL(Name) \
  mono_add_internal_call("EveEngine.InternalCalls::" #Name, Name)

static MonoObject* GetScriptInstance(GUUID entity_id) {
  return ScriptEngine::GetManagedInstance(entity_id);
}

static bool Entity_HasComponent(GUUID entity_id,
                                MonoReflectionType* component_type) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  MonoType* managed_type = mono_reflection_type_get_type(component_type);
  ASSERT(entity_has_component_funcs.find(managed_type) !=
         entity_has_component_funcs.end());
  return entity_has_component_funcs.at(managed_type)(entity.value());
}

static uint64_t Entity_FindEntityByName(MonoString* name) {
  char* name_cstr = mono_string_to_utf8(name);

  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByName(name_cstr);
  mono_free(name_cstr);

  if (!entity)
    return 0;

  return entity->GetUUID();
}

static void TransformComponent_GetPosition(GUUID entity_id,
                                           glm::vec3* out_position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  *out_position = entity->GetComponent<Transform>().position;
}

static void TransformComponent_SetPosition(GUUID entity_id,
                                           glm::vec3* position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  entity->GetComponent<Transform>().position = *position;
}

static void TransformComponent_GetRotation(GUUID entity_id,
                                           glm::vec3* out_rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  *out_rotation = entity->GetComponent<Transform>().rotation;
}

static void TransformComponent_SetRotation(GUUID entity_id,
                                           glm::vec3* rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  entity->GetComponent<Transform>().rotation = *rotation;
}

static void TransformComponent_GetScale(GUUID entity_id, glm::vec3* out_scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  *out_scale = entity->GetComponent<Transform>().scale;
}

static void TransformComponent_SetScale(GUUID entity_id, glm::vec3* scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  entity->GetComponent<Transform>().scale = *scale;
}

static void TransformComponent_GetForward(GUUID entity_id,
                                          glm::vec3* out_forward) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  // Calculate forward based on rotation, assuming Z is forward in this case
  *out_forward = entity->GetComponent<Transform>().GetForward();
}

static void TransformComponent_GetRight(GUUID entity_id, glm::vec3* out_right) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  // Calculate forward based on rotation, assuming Z is forward in this case
  *out_right = entity->GetComponent<Transform>().GetRight();
}

static void TransformComponent_GetUp(GUUID entity_id, glm::vec3* out_up) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->FindEntityByUUID(entity_id);
  ASSERT(entity.has_value());

  // Calculate forward based on rotation, assuming Z is forward in this case
  *out_up = entity->GetComponent<Transform>().GetUp();
}

static bool Input_IsKeyPressed(KeyCode keycode) {
  return Input::IsKeyPressed(keycode);
}

static bool Input_IsKeyReleased(KeyCode keycode) {
  return Input::IsKeyReleased(keycode);
}

static void SceneManager_SetActive(int index) {
  SceneManager::SetActive(index);
}

static int SceneManager_GetActiveIndex() {
  return SceneManager::GetActiveIndex();
}

template <typename... Component>
static void RegisterComponent() {
  (
      []() {
        std::string_view type_name = typeid(Component).name();
        size_t pos = type_name.find_last_of(' ');
        std::string_view struct_name = type_name.substr(pos + 1);
        std::string managed_type_name =
            std::format("EveEngine.{}", struct_name);

        MonoType* managed_type = mono_reflection_type_from_name(
            managed_type_name.data(), ScriptEngine::GetCoreAssemblyImage());
        if (!managed_type) {
          LOG_ERROR("Could not find component type {}", managed_type_name);
          return;
        }
        entity_has_component_funcs[managed_type] = [](Entity entity) {
          return entity.HasComponent<Component>();
        };
      }(),
      ...);
}

template <typename... Component>
static void RegisterComponent(ComponentGroup<Component...>) {
  RegisterComponent<Component...>();
}

void ScriptGlue::RegisterComponents() {
  entity_has_component_funcs.clear();
  RegisterComponent(AllComponents{});
}

void ScriptGlue::RegisterFunctions() {
  ADD_INTERNAL_CALL(GetScriptInstance);

  ADD_INTERNAL_CALL(Entity_HasComponent);
  ADD_INTERNAL_CALL(Entity_FindEntityByName);

  ADD_INTERNAL_CALL(TransformComponent_GetPosition);
  ADD_INTERNAL_CALL(TransformComponent_SetPosition);

  ADD_INTERNAL_CALL(TransformComponent_GetRotation);
  ADD_INTERNAL_CALL(TransformComponent_SetRotation);

  ADD_INTERNAL_CALL(TransformComponent_GetScale);
  ADD_INTERNAL_CALL(TransformComponent_SetScale);

  ADD_INTERNAL_CALL(TransformComponent_GetForward);
  ADD_INTERNAL_CALL(TransformComponent_GetRight);
  ADD_INTERNAL_CALL(TransformComponent_GetUp);

  ADD_INTERNAL_CALL(SceneManager_SetActive);
  ADD_INTERNAL_CALL(SceneManager_GetActiveIndex);

  ADD_INTERNAL_CALL(Input_IsKeyPressed);
  ADD_INTERNAL_CALL(Input_IsKeyReleased);
}
