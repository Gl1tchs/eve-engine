// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#include "pch_shared.h"

#include "scripting/script_glue.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include "core/event/input.h"
#include "core/event/key_code.h"
#include "core/uuid.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/transform.h"
#include "scripting/script_engine.h"

namespace eve {
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

static MonoObject* GetScriptInstance(UUID entity_id) {
  return ScriptEngine::GetManagedInstance(entity_id);
}

static bool Entity_HasComponent(UUID entity_id,
                                MonoReflectionType* component_type) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  MonoType* managed_type = mono_reflection_type_get_type(component_type);
  ASSERT(entity_has_component_funcs.find(managed_type) !=
         entity_has_component_funcs.end());
  return entity_has_component_funcs.at(managed_type)(entity);
}

static uint64_t Entity_TryGetEntityByName(MonoString* name) {
  char* name_cstr = mono_string_to_utf8(name);

  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByName(name_cstr);
  mono_free(name_cstr);

  if (!entity)
    return 0;

  return entity.GetUUID();
}

static void TransformComponent_GetPosition(UUID entity_id,
                                           glm::vec3* out_position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_position = entity.GetComponent<Transform>().position;
}

static void TransformComponent_SetPosition(UUID entity_id,
                                           glm::vec3* position) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Transform>().position = *position;
}

static void TransformComponent_GetRotation(UUID entity_id,
                                           glm::vec3* out_rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_rotation = entity.GetComponent<Transform>().rotation;
}

static void TransformComponent_SetRotation(UUID entity_id,
                                           glm::vec3* rotation) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Transform>().rotation = *rotation;
}

static void TransformComponent_GetScale(UUID entity_id, glm::vec3* out_scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_scale = entity.GetComponent<Transform>().scale;
}

static void TransformComponent_SetScale(UUID entity_id, glm::vec3* scale) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Transform>().scale = *scale;
}

static void TransformComponent_GetForward(UUID entity_id,
                                          glm::vec3* out_forward) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_forward = entity.GetComponent<Transform>().GetForward();
}

static void TransformComponent_GetRight(UUID entity_id, glm::vec3* out_right) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_right = entity.GetComponent<Transform>().GetRight();
}

static void TransformComponent_GetUp(UUID entity_id, glm::vec3* out_up) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_up = entity.GetComponent<Transform>().GetUp();
}

static void CameraComponent_OrthographicCamera_GetAspectRatio(
    UUID entity_id, float* out_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_aspect_ratio =
      entity.GetComponent<CameraComponent>().ortho_camera.aspect_ratio;
}

static void CameraComponent_OrthographicCamera_SetAspectRatio(
    UUID entity_id, float* aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.aspect_ratio =
      *aspect_ratio;
}

static void CameraComponent_OrthographicCamera_GetZoomLevel(
    UUID entity_id, float* out_zoom_level) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_zoom_level =
      entity.GetComponent<CameraComponent>().ortho_camera.zoom_level;
}

static void CameraComponent_OrthographicCamera_SetZoomLevel(UUID entity_id,
                                                            float* zoom_level) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.zoom_level = *zoom_level;
}

static void CameraComponent_OrthographicCamera_GetNearClip(
    UUID entity_id, float* out_near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_near_clip =
      entity.GetComponent<CameraComponent>().ortho_camera.near_clip;
}

static void CameraComponent_OrthographicCamera_SetNearClip(UUID entity_id,
                                                           float* near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.near_clip = *near_clip;
}

static void CameraComponent_OrthographicCamera_GetFarClip(UUID entity_id,
                                                          float* out_far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_far_clip = entity.GetComponent<CameraComponent>().ortho_camera.far_clip;
}

static void CameraComponent_OrthographicCamera_SetFarClip(UUID entity_id,
                                                          float* far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().ortho_camera.far_clip = *far_clip;
}

static void CameraComponent_PerspectiveCamera_GetAspectRatio(
    UUID entity_id, float* out_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_aspect_ratio =
      entity.GetComponent<CameraComponent>().persp_camera.aspect_ratio;
}

static void CameraComponent_PerspectiveCamera_SetAspectRatio(
    UUID entity_id, float* aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().persp_camera.aspect_ratio =
      *aspect_ratio;
}

static void CameraComponent_PerspectiveCamera_GetFov(UUID entity_id,
                                                     float* out_fov) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_fov = entity.GetComponent<CameraComponent>().persp_camera.fov;
}

static void CameraComponent_PerspectiveCamera_SetFov(UUID entity_id,
                                                     float* fov) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().persp_camera.fov = *fov;
}

static void CameraComponent_PerspectiveCamera_GetNearClip(
    UUID entity_id, float* out_near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_near_clip =
      entity.GetComponent<CameraComponent>().persp_camera.near_clip;
}

static void CameraComponent_PerspectiveCamera_SetNearClip(UUID entity_id,
                                                          float* near_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().persp_camera.near_clip = *near_clip;
}

static void CameraComponent_PerspectiveCamera_GetFarClip(UUID entity_id,
                                                         float* out_far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_far_clip = entity.GetComponent<CameraComponent>().persp_camera.far_clip;
}

static void CameraComponent_PerspectiveCamera_SetFarClip(UUID entity_id,
                                                         float* far_clip) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().persp_camera.far_clip = *far_clip;
  ;
}

static void CameraComponent_GetIsOrthographic(UUID entity_id,
                                              float* out_is_orthographic) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_is_orthographic = entity.GetComponent<CameraComponent>().is_orthographic;
}

static void CameraComponent_SetIsOrthographic(UUID entity_id,
                                              float* is_orthographic) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().is_orthographic = *is_orthographic;
}

static void CameraComponent_GetIsPrimary(UUID entity_id,
                                         float* out_is_primary) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_is_primary = entity.GetComponent<CameraComponent>().is_primary;
}

static void CameraComponent_SetIsPrimary(UUID entity_id, float* is_primary) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().is_primary = *is_primary;
}

static void CameraComponent_GetIsFixedAspectRato(
    UUID entity_id, float* out_is_fixed_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_is_fixed_aspect_ratio =
      entity.GetComponent<CameraComponent>().is_fixed_aspect_ratio;
}

static void CameraComponent_SetIsFixedAspectRato(UUID entity_id,
                                                 float* is_fixed_aspect_ratio) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<CameraComponent>().is_fixed_aspect_ratio =
      *is_fixed_aspect_ratio;
}

static void Material_GetAlbedo(UUID entity_id, glm::vec3* out_albedo) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_albedo = entity.GetComponent<Material>().albedo;
}

static void Material_SetAlbedo(UUID entity_id, glm::vec3* albedo) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Material>().albedo = *albedo;
}

static void Material_GetMetallic(UUID entity_id, float* out_metallic) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_metallic = entity.GetComponent<Material>().metallic;
}

static void Material_SetMetallic(UUID entity_id, float* metallic) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Material>().metallic = *metallic;
}

static void Material_GetRoughness(UUID entity_id, float* out_roughness) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_roughness = entity.GetComponent<Material>().roughness;
}

static void Material_SetRoughness(UUID entity_id, float* roughness) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Material>().roughness = *roughness;
}

static void Material_GetAO(UUID entity_id, float* out_ao) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  *out_ao = entity.GetComponent<Material>().ao;
}

static void Material_SetAO(UUID entity_id, float* ao) {
  Scene* scene = ScriptEngine::GetSceneContext();
  ASSERT(scene);
  auto entity = scene->TryGetEntityByUUID(entity_id);
  ASSERT(entity);

  entity.GetComponent<Material>().ao = *ao;
}

static bool Input_IsKeyPressed(KeyCode keycode) {
  return Input::IsKeyPressed(keycode);
}

static bool Input_IsKeyReleased(KeyCode keycode) {
  return Input::IsKeyReleased(keycode);
}

static bool Input_IsMouseButtonPressed(MouseCode mouse_code) {
  return Input::IsMouseButtonPressed(mouse_code);
}

static bool Input_IsMouseButtonReleased(MouseCode mouse_code) {
  return Input::IsMouseButtonReleased(mouse_code);
}

static void Input_GetMousePosition(glm::vec2* out_position) {
  *out_position = Input::GetMousePosition();
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
        size_t pos = type_name.find_last_of('::');
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

  // Begin Entity
  ADD_INTERNAL_CALL(Entity_HasComponent);
  ADD_INTERNAL_CALL(Entity_TryGetEntityByName);

  // Begin Transform Component
  ADD_INTERNAL_CALL(TransformComponent_GetPosition);
  ADD_INTERNAL_CALL(TransformComponent_SetPosition);

  ADD_INTERNAL_CALL(TransformComponent_GetRotation);
  ADD_INTERNAL_CALL(TransformComponent_SetRotation);

  ADD_INTERNAL_CALL(TransformComponent_GetScale);
  ADD_INTERNAL_CALL(TransformComponent_SetScale);

  ADD_INTERNAL_CALL(TransformComponent_GetForward);
  ADD_INTERNAL_CALL(TransformComponent_GetRight);
  ADD_INTERNAL_CALL(TransformComponent_GetUp);

  // Begin Camera Component
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetAspectRatio);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetAspectRatio);

  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetZoomLevel);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetZoomLevel);

  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetNearClip);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetNearClip);

  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_GetFarClip);
  ADD_INTERNAL_CALL(CameraComponent_OrthographicCamera_SetFarClip);

  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetAspectRatio);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetAspectRatio);

  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetFov);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetFov);

  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetNearClip);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetNearClip);

  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_GetFarClip);
  ADD_INTERNAL_CALL(CameraComponent_PerspectiveCamera_SetFarClip);

  ADD_INTERNAL_CALL(CameraComponent_GetIsOrthographic);
  ADD_INTERNAL_CALL(CameraComponent_SetIsOrthographic);

  ADD_INTERNAL_CALL(CameraComponent_GetIsPrimary);
  ADD_INTERNAL_CALL(CameraComponent_SetIsPrimary);

  ADD_INTERNAL_CALL(CameraComponent_GetIsFixedAspectRato);
  ADD_INTERNAL_CALL(CameraComponent_SetIsFixedAspectRato);

  // Begin Material
  ADD_INTERNAL_CALL(Material_GetAlbedo);
  ADD_INTERNAL_CALL(Material_SetAlbedo);

  ADD_INTERNAL_CALL(Material_GetMetallic);
  ADD_INTERNAL_CALL(Material_SetMetallic);

  ADD_INTERNAL_CALL(Material_GetRoughness);
  ADD_INTERNAL_CALL(Material_SetRoughness);

  ADD_INTERNAL_CALL(Material_GetAO);
  ADD_INTERNAL_CALL(Material_SetAO);

  // Begin Scene Manager
  ADD_INTERNAL_CALL(SceneManager_SetActive);
  ADD_INTERNAL_CALL(SceneManager_GetActiveIndex);

  // Begin Input
  ADD_INTERNAL_CALL(Input_IsKeyPressed);
  ADD_INTERNAL_CALL(Input_IsKeyReleased);
  ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
  ADD_INTERNAL_CALL(Input_IsMouseButtonReleased);
  ADD_INTERNAL_CALL(Input_GetMousePosition);
}
}  // namespace eve
